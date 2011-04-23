#include <algorithm>

#include "func-reference-type.h"
#include "expr-nodes.h"
#include "../output/name-mangler.h"
#include "../output/func-writer.h"
#include "../util/string.h"
#include "../util/map-compare.h"
#include "../proto/function.h"
#include "../proto/node-base.h"

using namespace inst;

std::string FuncReferenceType::exportedName() const
{
    return output::form_FuncReferenceType(size);
}

std::string FuncReferenceType::name() const
{
    return "Function reference [ "
         + _func_proto->name
         + " with "
         + util::str(int(_func_proto->param_names.size()))
         + " parameters ]";
}

bool FuncReferenceType::operator==(Type const& rhs) const
{
    return rhs.eqAsFuncReference(_func_proto, context_references);
}

bool FuncReferenceType::operator<(Type const& rhs) const
{
    return rhs.ltAsFuncReference(_func_proto, context_references);
}

bool FuncReferenceType::eqAsFuncReference(util::sref<proto::Function> lhs_func
                                             , std::map<std::string, Variable const> const& rhs_cr) const
{
    return _func_proto.id() == lhs_func.id() && context_references == rhs_cr;
}

bool FuncReferenceType::ltAsFuncReference(util::sref<proto::Function> lhs_func
                                             , std::map<std::string, Variable const> const& rhs_cr) const
{
    if (_func_proto.id() == lhs_func.id()) {
        return util::map_less(context_references, rhs_cr);
    }
    return _func_proto.id() < lhs_func.id();
}

bool FuncReferenceType::ltAsBuiltIn(Type const&) const
{
    return true;
}

std::map<std::string, Variable const> FuncReferenceType::_enclose_reference(
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
                      map.insert(std::make_pair(reference.first
                                              , Variable(pos, reference.second.type, offset, level)));
                      offset += reference.second.type->size;
                  });
    return map;
}

int FuncReferenceType::_calc_size(std::map<std::string, Variable const> const& cr)
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

util::sptr<inst::Expression const> FuncReferenceType::call(
            misc::position const&
          , int level
          , int stack_offset
          , std::vector<util::sref<inst::Type const>> const& arg_types
          , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(util::mkptr(new Call(_func_proto->inst(level, _adjust_vars(stack_offset, level), arg_types)
                                        , std::move(args))));
}

void FuncReferenceType::write() const
{
    output::constructFuncReference(exportedName());
    int offset = 0;
    std::for_each(context_references.begin()
                , context_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      output::funcReferenceNextVariable(
                                           offset
                                         , output::stack_var_record(reference.second.type->exportedName()
                                                                  , reference.second.stack_offset
                                                                  , reference.second.level));
                      offset += reference.second.type->size;
                  });
}

std::map<std::string, Variable const> FuncReferenceType::_adjust_vars(int stack_offset, int level) const
{
    std::map<std::string, Variable const> result;
    std::for_each(closed_references.begin()
                , closed_references.end()
                , [&](std::pair<std::string, Variable const> const& reference)
                  {
                      result.insert(std::make_pair(reference.first
                                                 , reference.second.adjust_location(stack_offset, level)));
                  });
    return result;
}
