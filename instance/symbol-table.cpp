#include <algorithm>

#include "symbol-table.h"
#include "variable.h"
#include "type.h"
#include "../report/errors.h"
#include "../misc/platform.h"

using namespace inst;

static variable const BAD_REF(misc::position(0), type::BIT_VOID, 0, 0);

symbol_table::symbol_table(int ext_lvl
                         , std::list<arg_name_type_pair> const& args
                         , std::map<std::string, variable const> const& ext_vars)
    : level(ext_lvl + 1)
    , _ss_used(0)
    , _external_defs(ext_vars)
{
    std::for_each(args.begin()
                , args.end()
                , [&](arg_name_type_pair const& arg_info)
                  {
                      _args.push_back(defVar(misc::position(0), arg_info.atype, arg_info.name));
                  });
}

static int calc_offset_on_align(int base, int new_size)
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

variable symbol_table::defVar(misc::position const& pos
                             , util::sref<type const> var_type
                             , std::string const& name)
{
    int offset = calc_offset_on_align(_ss_used, var_type->size);
    auto insert_result = _local_defs.insert(std::make_pair(name, variable(pos, var_type, offset, level)));
    _ss_used = offset + var_type->size;
    return insert_result.first->second;
}

variable symbol_table::query_var(misc::position const& pos, std::string const& name) const
{
    auto find_result = _local_defs.find(name);
    if (_local_defs.end() != find_result) {
        return find_result->second;
    }

    auto ext_find_result = _external_defs.find(name);
    if (_external_defs.end() != ext_find_result) {
        return ext_find_result->second;
    }

    error::var_not_def(pos, name);
    return BAD_REF;
}

int symbol_table::stack_size() const
{
    return _ss_used;
}

std::list<variable> symbol_table::get_args() const
{
    return _args;
}
