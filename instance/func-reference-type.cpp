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

std::string func_reference_type::exported_name() const
{
    return output::form_func_reference_type(size);
}

std::string func_reference_type::name() const
{
    return "Function reference [ "
         + _func_proto->name
         + " with "
         + util::str(int(_func_proto->param_names.size()))
         + " parameters ]";
}

bool func_reference_type::operator==(type const& rhs) const
{
    return rhs.eq_as_func_reference(_func_proto, context_references);
}

bool func_reference_type::operator<(type const& rhs) const
{
    return rhs.lt_as_func_reference(_func_proto, context_references);
}

bool func_reference_type::eq_as_func_reference(util::sref<proto::function> lhs_func
                                             , std::map<std::string, variable const> const& rhs_cr) const
{
    return _func_proto.id() == lhs_func.id() && context_references == rhs_cr;
}

bool func_reference_type::lt_as_func_reference(util::sref<proto::function> lhs_func
                                             , std::map<std::string, variable const> const& rhs_cr) const
{
    if (_func_proto.id() == lhs_func.id()) {
        return util::map_less(context_references, rhs_cr);
    }
    return _func_proto.id() < lhs_func.id();
}

bool func_reference_type::lt_as_built_in(type const&) const
{
    return true;
}

std::map<std::string, variable const> func_reference_type::_enclose_reference(
                                            misc::pos_type const& pos
                                          , int level
                                          , std::map<std::string, variable const> const& cr)
{
    std::map<std::string, variable const> map;
    int offset = 0;
    std::for_each(cr.begin()
                , cr.end()
                , [&](std::pair<std::string, variable const> const& reference)
                  {
                      map.insert(std::make_pair(reference.first
                                              , variable(pos, reference.second.vtype, offset, level)));
                      offset += reference.second.vtype->size;
                  });
    return map;
}

int func_reference_type::_calc_size(std::map<std::string, variable const> const& cr)
{
    int size = 0;
    std::for_each(cr.begin()
                , cr.end()
                , [&](std::pair<std::string, variable const> const& reference)
                  {
                      size += reference.second.vtype->size;
                  });
    return size;
}

util::sptr<inst::Expression const> func_reference_type::call_func(
            misc::pos_type const&
          , int level
          , int stack_offset
          , std::vector<util::sref<inst::type const>> const& arg_types
          , std::vector<util::sptr<Expression const>> args) const
{
    return std::move(util::mkptr(new call(_func_proto->inst(level, _adjust_vars(stack_offset, level), arg_types)
                                        , std::move(args))));
}

void func_reference_type::write() const
{
    output::construct_func_reference(exported_name());
    int offset = 0;
    std::for_each(context_references.begin()
                , context_references.end()
                , [&](std::pair<std::string, variable const> const& reference)
                  {
                      output::func_reference_next_variable(
                                           offset
                                         , output::stack_var_record(reference.second.vtype->exported_name()
                                                                  , reference.second.stack_offset
                                                                  , reference.second.level));
                      offset += reference.second.vtype->size;
                  });
}

std::map<std::string, variable const> func_reference_type::_adjust_vars(int stack_offset, int level) const
{
    std::map<std::string, variable const> result;
    std::for_each(closed_references.begin()
                , closed_references.end()
                , [&](std::pair<std::string, variable const> const& reference)
                  {
                      result.insert(std::make_pair(reference.first
                                                 , reference.second.adjust_location(stack_offset, level)));
                  });
    return result;
}
