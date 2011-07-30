#include <algorithm>

#include "function.h"
#include "symbol-table.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "variable.h"
#include "../instance/stmt-nodes.h"
#include "../util/map-compare.h"
#include "../util/vector-append.h"
#include "../report/errors.h"

using namespace proto;

util::sref<FuncInstDraft> Function::inst(misc::position const& pos
                                       , util::sref<SymbolTable const> ext_st
                                       , std::vector<util::sref<Type const>> const& arg_types)
{
    return inst(ext_st->level, bindExternalVars(pos, ext_st), arg_types);
}

util::sref<FuncInstDraft> Function::_draftInCacheOrNulIfNonexist(
                        std::map<std::string, Variable const> const& ext_vars
                      , std::vector<util::sref<Type const>> const& arg_types) const
{
    auto find_result = _draft_cache.find(ext_vars, arg_types);
    if (_draft_cache.end() == find_result) {
        return util::sref<FuncInstDraft>(nullptr);
    }
    util::sref<FuncInstDraft> draft = *(find_result->draft);
    while (!draft->isReturnTypeResolved() && draft->hasMorePath()) {
        draft->instNextPath();
    }
    if (!draft->isReturnTypeResolved()) {
        error::returnTypeUnresolvable(name, arg_types.size());
        draft->setReturnType(misc::position(0), Type::BAD_TYPE);
    }
    return draft;
}

std::list<ArgNameTypeRec> makeArgInfo(std::vector<std::string> const& param_names
                                    , std::vector<util::sref<Type const>> const& arg_types)
{
    std::list<ArgNameTypeRec> args;
    for (unsigned i = 0; i < arg_types.size(); ++i) {
        args.push_back(ArgNameTypeRec(param_names[i], arg_types[i]));
    }
    return args;
}

util::sref<FuncInstDraft> Function::inst(int level
                                       , std::map<std::string, Variable const> const& ext_vars
                                       , std::vector<util::sref<Type const>> const& arg_types)
{
    util::sref<FuncInstDraft> draft = _draftInCacheOrNulIfNonexist(ext_vars, arg_types);
    if (draft.not_nul()) {
        return draft;
    }

    util::sptr<FuncInstDraft> new_draft(FuncInstDraft::create(level
                                                            , makeArgInfo(param_names, arg_types)
                                                            , ext_vars
                                                            , hint_void_return));
    util::sref<FuncInstDraft> draft_ref(*new_draft);
    _draft_cache.append(DraftInfo(ext_vars, arg_types, std::move(new_draft)));
    draft_ref->instantiate(block());
    return draft_ref;
}

std::map<std::string, Variable const> Function::bindExternalVars(
                                                  misc::position const& pos
                                                , util::sref<SymbolTable const> ext_st) const
{
    std::map<std::string, Variable const> result;
    std::for_each(_free_variables.begin()
                , _free_variables.end()
                , [&](std::string const& var_name)
                  {
                      result.insert(std::make_pair(var_name, ext_st->queryVar(pos, var_name)));
                  });
    return result;
}

util::sref<FuncReferenceType const> Function::refType(misc::position const& reference_pos
                                                    , util::sref<SymbolTable const> st)
{
    std::map<std::string, Variable const> ext_vars(bindExternalVars(reference_pos, st));
    auto find_result = std::find_if(_reference_types.begin()
                                  , _reference_types.end()
                                  , [&](util::sptr<FuncReferenceType const> const& type)
                                    {
                                        return type->context_references == ext_vars;
                                    });
    if (find_result != _reference_types.end()) {
        return **find_result;
    }
    _reference_types.push_back(util::mkptr(
                    new FuncReferenceType(reference_pos, util::mkref(*this), st->level, ext_vars)));
    return *_reference_types.back();
}

void Function::setFreeVariables(std::vector<std::string> const& free_vars)
{
    _free_variables = free_vars;
}

std::vector<util::sptr<inst::Function const>> Function::deliverFuncs()
{
    return util::ptrs_append(_block.deliverFuncs(), _draft_cache.deliverFuncs());
}

util::sref<Block> Function::block()
{
    return util::mkref(_block);
}

Function::DraftCache::Iterator
        Function::DraftCache::find(std::map<std::string, Variable const> const& ext_vars
                                 , std::vector<util::sref<Type const>> const& arg_types) const
{
    return std::find_if(BaseType::begin()
                      , BaseType::end()
                      , [&](DraftInfo const& info)
                        {
                            return ext_vars == info.ext_vars && arg_types == info.arg_types;
                        });
}

Function::DraftCache::Iterator Function::DraftCache::end() const
{
    return std::list<DraftInfo>::end();
}

void Function::DraftCache::append(DraftInfo info)
{
    push_back(std::move(info));
}

std::vector<util::sptr<inst::Function const>> Function::DraftCache::deliverFuncs()
{
    std::vector<util::sptr<inst::Function const>> result;
    std::for_each(BaseType::begin()
                , BaseType::end()
                , [&](DraftInfo& info)
                  {
                      result.push_back(std::move(info.draft->deliver()));
                  });
    return std::move(result);
}
