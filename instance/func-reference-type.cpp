#include <algorithm>

#include "func-reference-type.h"
#include "../util/string.h"
#include "../util/map-compare.h"
#include "../proto/function.h"
#include "../proto/node-base.h"

using namespace inst;

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

util::sref<proto::function> func_reference_type::get_func_proto() const
{
    return _func_proto;
}

std::map<std::string, variable const> func_reference_type::_enclose_reference(
                                            misc::pos_type const& pos
                                          , int references_offset
                                          , std::map<std::string, variable const> const& cr)
{
    std::map<std::string, variable const> map;
    int offset = references_offset;
    std::for_each(cr.begin()
                , cr.end()
                , [&](std::pair<std::string, variable const> const& reference)
                  {
                      map.insert(std::make_pair(reference.first
                                              , variable(pos, reference.second.vtype, offset)));
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
