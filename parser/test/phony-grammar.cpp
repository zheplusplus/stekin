#include <algorithm>

#include "data-trees.h"

#include "../../grammar/expr-nodes.h"
#include "../../grammar/clause-builder.h"

using namespace grammar;
using test::data_node_base;

proto::expr_prototype const* pre_unary_oper::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_BEGIN, op_img)
                                 (pos, data_node_base::OPERAND);
    rhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_END);
    return NULL;
}

proto::expr_prototype const* binary_oper::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, op_img)(pos, data_node_base::OPERAND);
    lhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return NULL;
}

proto::expr_prototype const* conjunction::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, "&&")(pos, data_node_base::OPERAND);
    lhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return NULL;
}

proto::expr_prototype const* disjunction::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, "||")(pos, data_node_base::OPERAND);
    lhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return NULL;
}

proto::expr_prototype const* negation::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_BEGIN, "!")(pos, data_node_base::OPERAND);
    rhs->compile(NULL);
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_END);
    return NULL;
}

proto::expr_prototype const* reference::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::IDENTIFIER, name);
    return NULL;
}

proto::expr_prototype const* int_literal::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::INTEGER, value);
    return NULL;
}

proto::expr_prototype const* float_literal::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::FLOATING, value);
    return NULL;
}

proto::expr_prototype const* call::compile(proto::prototype_scope const*) const
{
    test::data_tree::actual_one()(pos, data_node_base::FUNC_CALL_BEGIN, name);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      test::data_tree::actual_one()(pos, data_node_base::ARGUMENT);
                      expr->compile(NULL);
                  });
    test::data_tree::actual_one()(pos, data_node_base::FUNC_CALL_END);
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
