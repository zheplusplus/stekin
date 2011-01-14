#include "node-factory.h"

using namespace proto;

int_literal const* node_factory::make_int(misc::pos_type const& pos, std::string const& value)
{
    _ints.push_back(int_literal(pos, value));
    return &_ints.back();
}

float_literal const* node_factory::make_float(misc::pos_type const& pos, std::string const& value)
{
    _floats.push_back(float_literal(pos, value));
    return &_floats.back();
}

reference const* node_factory::make_ref(misc::pos_type const& pos, std::string const& name)
{
    _refs.push_back(reference(pos, name));
    return &_refs.back();
}

call const* node_factory::call_func(misc::pos_type const& pos
                                  , func_templ* func
                                  , std::vector<expr_base const*> const& args)
{
    _calls.push_back(call(pos, func, args));
    return &_calls.back();
}

binary_op const* node_factory::make_binary(misc::pos_type const& pos
                                         , expr_base const* lhs
                                         , std::string const& op
                                         , expr_base const* rhs)
{
    _binaries.push_back(binary_op(pos, lhs, op, rhs));
    return &_binaries.back();
}

pre_unary_op const*
    node_factory::make_pre_unary(misc::pos_type const& pos, std::string const& op, expr_base const* rhs)
{
    _pre_unaries.push_back(pre_unary_op(pos, op, rhs));
    return &_pre_unaries.back();
}

conjunction const* node_factory::make_conj(misc::pos_type const& pos
                                         , expr_base const* lhs
                                         , expr_base const* rhs)
{
    _conjs.push_back(conjunction(pos, lhs, rhs));
    return &_conjs.back();
}

disjunction const* node_factory::make_disj(misc::pos_type const& pos
                                         , expr_base const* lhs
                                         , expr_base const* rhs)
{
    _disjs.push_back(disjunction(pos, lhs, rhs));
    return &_disjs.back();
}

negation const* node_factory::make_nega(misc::pos_type const& pos, expr_base const* rhs)
{
    _negas.push_back(negation(pos, rhs));
    return &_negas.back();
}

func_ret const* node_factory::make_ret(misc::pos_type const& pos, expr_base const* ret_val)
{
    _returns.push_back(func_ret(pos, ret_val));
    return &_returns.back();
}

func_ret_nothing const* node_factory::make_ret_nothing(misc::pos_type const& pos)
{
    _void_returns.push_back(func_ret_nothing(pos));
    return &_void_returns.back();
}

arithmetics const* node_factory::make_arith(misc::pos_type const& pos, expr_base const* expr)
{
    _ariths.push_back(arithmetics(pos, expr));
    return &_ariths.back();
}

branch const* node_factory::make_branch(misc::pos_type const& pos
                                      , expr_base const* condition
                                      , stmt_base const* valid
                                      , stmt_base const* invalid)
{
    _branches.push_back(branch(pos, condition, valid, invalid));
    return &_branches.back();
}

loop const* node_factory::make_loop(misc::pos_type const& pos
                                  , expr_base const* condition
                                  , stmt_base const* body)
{
    _loops.push_back(loop(pos, condition, body));
    return &_loops.back();
}

var_def const* node_factory::def_var(misc::pos_type const& pos
                                   , std::string const& var_name
                                   , expr_base const* var_init)
{
    _var_defs.push_back(var_def(pos, var_name, var_init));
    return &_var_defs.back();
}

block* node_factory::create_block()
{
    _blocks.push_back(block());
    return &_blocks.back();
}
