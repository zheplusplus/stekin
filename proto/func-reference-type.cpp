#include <algorithm>

#include "func-reference-type.h"
#include "function.h"
#include "block.h"
#include "node-base.h"
#include "func-inst-draft.h"
#include "variable.h"
#include "../instance/expr-nodes.h"
#include "../output/name-mangler.h"
#include "../output/func-writer.h"
#include "../util/string.h"
#include "../util/map-compare.h"

using namespace proto;

std::string FuncReferenceType::exportedName() const
{
    return output::formFuncReferenceType(size);
}

std::string FuncReferenceType::name() const
{
    return "Function reference [ "
         + _func->name
         + " with "
         + util::str(int(_func->param_names.size()))
         + " parameters ]";
}

bool FuncReferenceType::operator==(Type const& rhs) const
{
    return rhs.eqAsFuncReference(_func, context_references);
}

bool FuncReferenceType::operator<(Type const& rhs) const
{
    return rhs.ltAsFuncReference(_func, context_references);
}

bool FuncReferenceType::eqAsFuncReference(
            util::sref<Function> lhs_func
          , std::map<std::string, Variable const> const& rhs_cr) const
{
    return _func.id() == lhs_func.id() && context_references == rhs_cr;
}

bool FuncReferenceType::ltAsFuncReference(
            util::sref<Function> lhs_func
          , std::map<std::string, Variable const> const& rhs_cr) const
{
    if (_func.id() == lhs_func.id()) {
        return util::map_less(context_references, rhs_cr);
    }
    return _func.id() < lhs_func.id();
}

bool FuncReferenceType::ltAsBuiltIn(Type const&) const
{
    return true;
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
                                misc::position const&
                              , int level
                              , int stack_offset
                              , std::vector<util::sref<Type const>> const& arg_types) const
{
    return _func->inst(level, _adjustVars(stack_offset, level), arg_types);
}

std::vector<inst::FuncReference::ArgInfo> FuncReferenceType::makeCallArgs() const
{
    std::list<inst::FuncReference::ArgInfo> result;
    int offset = 0;
    std::for_each(context_references.begin()
                , context_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      result.push_back(inst::FuncReference::ArgInfo(
                                                          reference.second.level
                                                        , reference.second.stack_offset
                                                        , reference.second.type->exportedName()
                                                        , offset));
                      offset += reference.second.type->size;
                  });
    return std::vector<inst::FuncReference::ArgInfo>(result.begin(), result.end());
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
