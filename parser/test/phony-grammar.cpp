#include <algorithm>

#include "data-trees.h"

#include "../../grammar/expr-nodes.h"
#include "../../grammar/clause-builder.h"

using namespace grammar;
using namespace test;

static util::sptr<proto::expr_base const> nullptr()
{
    return std::move(util::sptr<proto::expr_base const>(0));
}

static util::sref<proto::scope> nullref()
{
    return util::sref<proto::scope>(0);
}

util::sptr<proto::expr_base const> pre_unary_oper::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_BEGIN, op_img)
                                 (pos, data_node_base::OPERAND);
    rhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_END);
    return std::move(nullptr());
}

util::sptr<proto::expr_base const> binary_oper::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, op_img)(pos, data_node_base::OPERAND);
    lhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> conjunction::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, "&&")(pos, data_node_base::OPERAND);
    lhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> disjunction::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_BEGIN, "||")(pos, data_node_base::OPERAND);
    lhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::OPERAND);
    rhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> negation::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_BEGIN, "!")(pos, data_node_base::OPERAND);
    rhs->compile(nullref());
    test::data_tree::actual_one()(pos, data_node_base::PRE_UNARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> reference::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::IDENTIFIER, name);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> bool_literal::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::BOOLEAN, value ? "true" : "false");
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> int_literal::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::INTEGER, value);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> float_literal::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::FLOATING, value);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> call::compile(util::sref<proto::scope>) const
{
    test::data_tree::actual_one()(pos, data_node_base::FUNC_CALL_BEGIN, name);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      test::data_tree::actual_one()(pos, data_node_base::ARGUMENT);
                      expr->compile(nullref());
                  });
    test::data_tree::actual_one()(pos, data_node_base::FUNC_CALL_END);
    return std::move(nullptr());;
}

void clause_builder::add_arith(int indent_level, util::sptr<expr_base const> arith)
{
    test::data_tree::actual_one()(arith->pos, indent_level, data_node_base::ARITHMETICS);
    util::sptr<expr_base const>(std::move(arith))->compile(nullref());
}

void clause_builder::add_var_def(int indent_level, std::string const& name, util::sptr<expr_base const> init)
{
    test::data_tree::actual_one()(init->pos, indent_level, data_node_base::VAR_DEF, name);
    util::sptr<expr_base const>(std::move(init))->compile(nullref());
}

void clause_builder::add_return(int indent_level, util::sptr<expr_base const> ret_val)
{
    test::data_tree::actual_one()(ret_val->pos, indent_level, data_node_base::RETURN);
    util::sptr<expr_base const>(std::move(ret_val))->compile(nullref());
}

void clause_builder::add_void_return(int indent_level, misc::pos_type const& pos)
{
    test::data_tree::actual_one()(pos, indent_level, data_node_base::RETURN, "");
}

void clause_builder::add_func_def(int indent_level
                                , misc::pos_type const& pos
                                , std::string const& name
                                , std::vector<std::string> const& params)
{
    test::data_tree::actual_one()(pos, indent_level, data_node_base::FUNC_DEF_HEAD_BEGIN, name);
    std::for_each(params.begin()
                , params.end()
                , [&](std::string const& param)
                  {
                      test::data_tree::actual_one()(pos, indent_level, data_node_base::IDENTIFIER, param);
                  });
    test::data_tree::actual_one()(pos, indent_level, data_node_base::FUNC_DEF_HEAD_END);
}

void clause_builder::add_if(int indent_level, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    test::data_tree::actual_one()(pos, indent_level, data_node_base::BRANCH_IF)
                                 (pos, indent_level, data_node_base::CONDITION_BEGIN);
    util::sptr<expr_base const>(std::move(condition))->compile(nullref());
    test::data_tree::actual_one()(pos, indent_level, data_node_base::CONDITION_END);
}

void clause_builder::add_ifnot(int indent_level, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    test::data_tree::actual_one()(pos, indent_level, data_node_base::BRANCH_IFNOT)
                                 (pos, indent_level, data_node_base::CONDITION_BEGIN);
    util::sptr<expr_base const>(std::move(condition))->compile(nullref());
    test::data_tree::actual_one()(pos, indent_level, data_node_base::CONDITION_END);
}

void clause_builder::add_else(int indent_level, misc::pos_type const& pos)
{
    test::data_tree::actual_one()(pos, indent_level, data_node_base::BRANCH_ELSE);
}

void clause_builder::add_while(int indent_level, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    test::data_tree::actual_one()(pos, indent_level, data_node_base::LOOP_WHILE)
                                 (pos, indent_level, data_node_base::CONDITION_BEGIN);
    util::sptr<expr_base const>(std::move(condition))->compile(nullref());
    test::data_tree::actual_one()(pos, indent_level, data_node_base::CONDITION_END);
}

void clause_builder::build_and_clear() {}

acceptor_stack::acceptor_stack() = default;
