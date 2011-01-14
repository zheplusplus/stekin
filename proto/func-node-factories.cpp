#include "func-node-factories.h"
#include "func-templ.h"
#include "err-report.h"

using namespace proto;

scope const* func_node_factory::create_branch_scope()
{
    _sub_scopes.push_back(scope(_create_sub_factory(), create_block(), _symbols));
    return &_sub_scopes.back();
}

scope const* func_node_factory::create_loop_scope()
{
    return create_branch_scope();
}

func_node_sub_factory* func_node_factory::_create_sub_factory()
{
    _sub_factories.push_back(std::move(func_node_sub_factory(_symbols)));
    return &_sub_factories.back();
}

var_def const*
    func_node_sub_factory::def_var(misc::pos_type const& pos, std::string const& name, expr_base const*)
{
    forbid_def_var(pos, name);
    return NULL;
}
