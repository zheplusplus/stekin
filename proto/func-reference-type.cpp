#include <algorithm>

#include <instance/expr-nodes.h>
#include <report/errors.h>
#include <util/string.h>

#include "func-reference-type.h"
#include "function.h"
#include "block.h"
#include "node-base.h"
#include "func-inst-draft.h"
#include "variable.h"
#include "list-types.h"

using namespace proto;

util::sptr<inst::Type const> FuncReferenceType::makeInstType() const
{
    std::vector<util::sptr<inst::Type const>> enclosed_types;
    std::for_each(context_references.begin()
                , context_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      enclosed_types.push_back(reference.second.type->makeInstType());
                  });
    return util::mkptr(new inst::ClosureType(size, std::move(enclosed_types)));
}

std::string FuncReferenceType::name() const
{
    return "Function reference [ "
         + _func->name
         + " with "
         + util::str(int(_func->param_names.size()))
         + " parameters ]";
}

std::map<std::string, Variable const> FuncReferenceType::_encloseReference(
                                            misc::position const& pos
                                          , int level
                                          , std::map<std::string, Variable const> const& cr)
{
    std::map<std::string, Variable const> map;
    int offset = 0;
    std::for_each(cr.begin()
                , cr.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      map.insert(std::make_pair(
                                        reference.first
                                      , Variable(pos, reference.second.type, offset, level)));
                      offset += reference.second.type->size;
                      if (ListType::isListType(reference.second.type)) {
                          error::featureNotSupportWrapListInClosure(pos);
                      }
                  });
    return map;
}

int FuncReferenceType::_calcSize(std::map<std::string, Variable const> const& cr)
{
    int size = 0;
    std::for_each(cr.begin()
                , cr.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      size += reference.second.type->size;
                  });
    return size;
}

util::sref<FuncInstDraft> FuncReferenceType::call(
                                int level
                              , int stack_offset
                              , std::vector<util::sref<Type const>> const& arg_types
                              , misc::trace& trace) const
{
    if (arg_types.size() != _func->param_names.size()) {
        error::callVariableArgCountWrong(trace.top(), arg_types.size(), _func->param_names.size());
        return FuncInstDraft::badDraft();
    }
    return _func->inst(level, _adjustVars(stack_offset, level), arg_types, trace);
}

std::list<inst::FuncReference::ArgInfo> FuncReferenceType::makeCallArgs() const
{
    std::list<inst::FuncReference::ArgInfo> result;
    int offset = 0;
    std::for_each(context_references.begin()
                , context_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      result.push_back(inst::FuncReference::ArgInfo(
                                inst::Address(reference.second.level, reference.second.stack_offset)
                              , reference.second.type->makeInstType()
                              , offset));
                      offset += reference.second.type->size;
                  });
    return result;
}

std::map<std::string, Variable const>
            FuncReferenceType::_adjustVars(int stack_offset, int level) const
{
    std::map<std::string, Variable const> result;
    std::for_each(closed_references.begin()
                , closed_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      result.insert(std::make_pair(reference.first
                                                 , reference.second.adjustLocation(stack_offset
                                                                                 , level)));
                  });
    return result;
}
