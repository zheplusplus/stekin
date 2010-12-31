#include "../../grammar/expr-nodes.h"
#include "../../grammar/clause-builder.h"

using namespace grammar;

proto::expr_prototype const* pre_unary_oper::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* binary_oper::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* conjunction::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* disjunction::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* negation::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* variable::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* int_literal::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* float_literal::compile(proto::prototype_scope const*) const
{
    return NULL;
}

proto::expr_prototype const* func_call::compile(proto::prototype_scope const*) const
{
    return NULL;
}

void clause_builder::add_arith(int indent_len, util::sptr<expr_base const>&& arith)
{
}

void clause_builder::add_var_def(int indent_len, std::string const& name, util::sptr<expr_base const>&& init)
{
}

void clause_builder::add_return(int indent_len, util::sptr<expr_base const>&& ret_val)
{
}

void clause_builder::add_void_return(int indent_len, misc::pos_type const& pos)
{
}

void clause_builder::add_func_def(int indent_len
                                , misc::pos_type const& pos
                                , std::string const& name
                                , std::vector<std::string> const& params)
{
}

void clause_builder::add_if(int indent_len, util::sptr<expr_base const>&& condition)
{
}

void clause_builder::add_if_not(int indent_len, util::sptr<expr_base const>&& condition)
{
}

void clause_builder::add_else(int indent_len, misc::pos_type const& pos)
{
}

void clause_builder::add_while(int indent_len, util::sptr<expr_base const>&& condition)
{
}

void clause_builder::build_and_clear()
{
}
