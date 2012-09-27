#include <algorithm>

#include <report/errors.h>
#include <misc/platform.h>

#include "symbol-table.h"
#include "operation.h"
#include "variable.h"
#include "type.h"

using namespace proto;

static Variable const BAD_REF(misc::position(), Type::s_void(), 0, 0);

SymbolTable::SymbolTable(int ext_lvl
                       , std::list<ArgNameTypeRec> const& args
                       , std::map<std::string, Variable const> const& ext_vars)
    : level(ext_lvl + 1)
    , _ss_used(0)
    , _external_defs(ext_vars)
{
    std::for_each(args.begin()
                , args.end()
                , [&](ArgNameTypeRec const& arg_info)
                  {
                      _args.push_back(defVar(misc::position(), arg_info.atype, arg_info.name));
                  });
}

static int calcOffsetOnAlign(int base, int new_size)
{
    int mod = base % platform::WORD_LENGTH_INBYTE;
    if (0 == mod) {
        return base;
    }
    if (mod + new_size <= platform::WORD_LENGTH_INBYTE) {
        return base;
    }
    return base - mod + platform::WORD_LENGTH_INBYTE;
}

static void mergeResEntries(std::vector<int>& a, std::vector<int> b)
{
    std::for_each(b.begin()
                , b.end()
                , [&](int entry)
                  {
                      a.push_back(entry);
                  });
}

Variable SymbolTable::defVar(misc::position const& pos
                           , util::sref<Type const> var_type
                           , std::string const& name)
{
    int offset = calcOffsetOnAlign(_ss_used, var_type->size);
    auto insert_result = _local_defs.insert(
            std::make_pair(name, Variable(pos, var_type, offset, level)));
    _ss_used = offset + var_type->size;
    mergeResEntries(_res_entries, var_type->resEntries(offset));
    return insert_result.first->second;
}

Variable SymbolTable::queryVar(misc::position const& pos, std::string const& name) const
{
    auto find_result = _local_defs.find(name);
    if (_local_defs.end() != find_result) {
        return find_result->second;
    }

    auto ext_find_result = _external_defs.find(name);
    if (_external_defs.end() != ext_find_result) {
        return ext_find_result->second;
    }

    error::varNotDef(pos, name);
    return BAD_REF;
}

util::sref<Operation const> SymbolTable::queryBinary(misc::position const& pos
                                                   , std::string const& op
                                                   , util::sref<Type const> lhs
                                                   , util::sref<Type const> rhs) const
{
    return Operation::queryBinary(pos, op, lhs, rhs);
}

util::sref<Operation const> SymbolTable::queryPreUnary(misc::position const& pos
                                                     , std::string const& op
                                                     , util::sref<Type const> rhs) const
{
    return Operation::queryPreUnary(pos, op, rhs);
}

int SymbolTable::stackSize() const
{
    return _ss_used;
}

std::list<Variable> SymbolTable::getArgs() const
{
    return _args;
}

std::vector<int> SymbolTable::getResEntries() const
{
    return _res_entries;
}
