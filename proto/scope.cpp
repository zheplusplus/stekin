#include "scope.h"
#include "stmt-nodes.h"
#include "func-templ.h"
#include "node-factory.h"

using namespace proto;

expr_base const* scope::make_int(misc::pos_type const& pos, std::string const& value) const
{
    return _factory->make_int(pos, value);
}

expr_base const* scope::make_float(misc::pos_type const& pos, std::string const& value) const
{
    return _factory->make_float(pos, value);
}

expr_base const* scope::make_ref(misc::pos_type const& pos, std::string const& name) const
{
    _symbols->ref_var(pos, name);
    return _factory->make_ref(pos, name);
}

expr_base const* scope::make_call(misc::pos_type const& pos
                                , std::string const& name
                                , std::vector<expr_base const*> const& args) const
{
    return _factory->call_func(pos, _symbols->query_func(pos, name, args.size()), args);
}

expr_base const* scope::make_binary(misc::pos_type const& pos
                                  , expr_base const* lhs
                                  , std::string const& op
                                  , expr_base const* rhs) const
{
    return _factory->make_binary(pos, lhs, op, rhs);
}

expr_base const* scope::make_pre_unary(misc::pos_type const& pos
                                     , std::string const& op
                                     , expr_base const* rhs) const
{
    return _factory->make_pre_unary(pos, op, rhs);
}

expr_base const* scope::make_conj(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs) const
{
    return _factory->make_conj(pos, lhs, rhs);
}

expr_base const* scope::make_disj(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs) const
{
    return _factory->make_disj(pos, lhs, rhs);
}

expr_base const* scope::make_nega(misc::pos_type const& pos, expr_base const* rhs) const
{
    return _factory->make_nega(pos, rhs);
}

void scope::func_ret(misc::pos_type const& pos, expr_base const* ret_val) const
{
    _block->add_stmt(_factory->make_ret(pos, ret_val));
}

void scope::func_ret_nothing(misc::pos_type const& pos) const
{
    _block->add_stmt(_factory->make_ret_nothing(pos));
}

void scope::make_arith(misc::pos_type const& pos, expr_base const* expr) const
{
    _block->add_stmt(_factory->make_arith(pos, expr));
}

void scope::branch(misc::pos_type const& pos
                 , expr_base const* condition
                 , stmt_base const* valid
                 , stmt_base const* invalid) const
{
    _block->add_stmt(_factory->make_branch(pos, condition, valid, invalid));
}

void scope::loop(misc::pos_type const& pos, expr_base const* condition, stmt_base const* body) const
{
    _block->add_stmt(_factory->make_loop(pos, condition, body));
}

void scope::def_var(misc::pos_type const& pos, std::string const& name, expr_base const* init) const
{
    _symbols->def_var(pos, name);
    _block->add_stmt(_factory->def_var(pos, name, init));
}

scope const* scope::create_branch_scope() const
{
    return _factory->create_branch_scope();
}

scope const* scope::create_loop_scope() const
{
    return _factory->create_loop_scope();
}

func_templ* scope::decl_func(misc::pos_type const& pos
                           , std::string const& name
                           , std::vector<std::string> const& param_names) const
{
    return _symbols->def_func(pos, name, param_names);
}

stmt_base const* scope::as_stmt() const
{
    return _block;
}

symbol_table* scope::get_symbols() const
{
    return _symbols;
}

scope const* scope::global_scope()
{
    static func_templ gscope(misc::pos_type(0), "", std::vector<std::string>());
    return &gscope.body_scope;
}
