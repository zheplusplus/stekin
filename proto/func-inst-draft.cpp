#include <list>
#include <set>
#include <algorithm>

#include <instance/node-base.h>
#include <report/errors.h>
#include <util/pointer.h>

#include "func-inst-draft.h"
#include "node-base.h"
#include "type.h"
#include "list-types.h"
#include "variable.h"

using namespace proto;

namespace {

    struct GlobalFuncInstDraft
        : public FuncInstDraft
    {
        GlobalFuncInstDraft() = default;
    };

    struct FuncInstDraftUnresolved
        : public FuncInstDraft
    {
        FuncInstDraftUnresolved(int ext_lvl
                              , std::list<ArgNameTypeRec> const& args
                              , std::map<std::string, Variable const> const& extvars)
            : FuncInstDraft(ext_lvl, args, extvars)
            , _return_type_or_nul_if_not_set(nullptr)
        {}

        util::sref<Type const> getReturnType() const
        {
            if (_return_type_or_nul_if_not_set.nul()) {
                return Type::BAD_TYPE;
            }
            return _return_type_or_nul_if_not_set;
        }

        void setReturnType(util::sref<Type const> return_type, misc::trace& trace)
        {
            if (_return_type_or_nul_if_not_set.nul()
                || Type::BAD_TYPE == _return_type_or_nul_if_not_set)
            {
                _return_type_or_nul_if_not_set = return_type;
                return;
            }
            if (_return_type_or_nul_if_not_set == return_type) {
                return;
            }
            util::sref<Type const> common_list_type(
                      ListType::commonListTypeOrNulIfImcompatible(return_type
                                                                , _return_type_or_nul_if_not_set));
            if (common_list_type.not_nul()) {
                _return_type_or_nul_if_not_set = common_list_type;
                return;
            }
            error::conflictReturnType(_return_type_or_nul_if_not_set->name()
                                    , return_type->name()
                                    , trace);
        }

        bool isReturnTypeResolved() const
        {
            return _return_type_or_nul_if_not_set.not_nul();
        }

        util::sref<Type const> _return_type_or_nul_if_not_set;
    };

}

util::sref<Type const> FuncInstDraft::getReturnType() const
{
    return Type::BIT_VOID;
}

void FuncInstDraft::setReturnType(util::sref<Type const> return_type, misc::trace& trace)
{
    if (Type::BIT_VOID != return_type) {
        error::conflictReturnType(Type::BIT_VOID->name(), return_type->name(), trace);
    }
}

bool FuncInstDraft::isReturnTypeResolved() const
{
    return true;
}

util::sptr<FuncInstDraft> FuncInstDraft::create(int ext_lvl
                                              , std::list<ArgNameTypeRec> const& args
                                              , std::map<std::string, Variable const> const& extvars
                                              , bool has_void_returns)
{
    return util::mkptr(has_void_returns ? new FuncInstDraft(ext_lvl, args, extvars)
                                        : new FuncInstDraftUnresolved(ext_lvl, args, extvars));
}

util::sptr<FuncInstDraft> FuncInstDraft::createGlobal()
{
    return util::mkptr(new GlobalFuncInstDraft);
}

util::sref<FuncInstDraft> FuncInstDraft::badDraft()
{
    static FuncInstDraft bad_draft;
    return util::mkref(bad_draft);
}

void FuncInstDraft::addPath(util::sref<Statement> path)
{
    _candidate_paths.push_back(path);
}

void FuncInstDraft::instNextPath(misc::trace& trace)
{
    if (!hasMorePath()) {
        return;
    }
    util::sref<Statement> next_path = _candidate_paths.front();
    _candidate_paths.pop_front();
    next_path->mediateInst(util::mkref(*this), trace);
}

bool FuncInstDraft::hasMorePath() const
{
    return !_candidate_paths.empty();
}

static std::list<inst::Function::ParamInfo> varsToParams(std::list<Variable> const& args)
{
    std::list<inst::Function::ParamInfo> params;
    std::for_each(args.begin()
                , args.end()
                , [&](Variable const& var)
                  {
                      params.push_back(inst::Function::ParamInfo(var.type->makeInstType()
                                                               , inst::Address(var.level
                                                                             , var.stack_offset)));
                  });
    return std::move(params);
}

util::sptr<inst::Function const> FuncInstDraft::deliver()
{
    return std::move(_inst_func_or_nul_if_not_inst);
}

util::sref<SymbolTable> FuncInstDraft::getSymbols()
{
    return util::mkref(_symbols);
}

int FuncInstDraft::level() const
{
    return _symbols.level;
}

void FuncInstDraft::instantiate(util::sref<Statement> stmt, misc::trace& trace)
{
    addPath(stmt);
    instNextPath(trace);
    util::sptr<inst::Statement const> body(stmt->inst(util::mkref(*this), trace));
    _inst_func_or_nul_if_not_inst.reset(new inst::Function(getReturnType()->makeInstType()
                                                         , _symbols.level
                                                         , _symbols.stackSize()
                                                         , varsToParams(_symbols.getArgs())
                                                         , sn
                                                         , _symbols.getResEntries()
                                                         , std::move(body)));
}
