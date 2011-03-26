#include <algorithm>

#include "test-common.h"
#include "../../test/data-trees.h"
#include "../../grammar/expr-nodes.h"
#include "../../grammar/clause-builder.h"
#include "../../flowcheck/block.h"
#include "../../flowcheck/function.h"
#include "../../flowcheck/node-base.h"
#include "../../proto/node-base.h"
#include "../../util/string.h"

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

util::sptr<proto::expr_base const> pre_unary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP_BEGIN, op_img)(pos, OPERAND);
    rhs->compile(nullref());
    data_tree::actual_one()(pos, PRE_UNARY_OP_END);
    return std::move(nullptr());
}

util::sptr<proto::expr_base const> binary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP_BEGIN, op_img)(pos, OPERAND);
    lhs->compile(nullref());
    data_tree::actual_one()(pos, OPERAND);
    rhs->compile(nullref());
    data_tree::actual_one()(pos, BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> conjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP_BEGIN, "&&")(pos, OPERAND);
    lhs->compile(nullref());
    data_tree::actual_one()(pos, OPERAND);
    rhs->compile(nullref());
    data_tree::actual_one()(pos, BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> disjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP_BEGIN, "||")(pos, OPERAND);
    lhs->compile(nullref());
    data_tree::actual_one()(pos, OPERAND);
    rhs->compile(nullref());
    data_tree::actual_one()(pos, BINARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> negation::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP_BEGIN, "!")(pos, OPERAND);
    rhs->compile(nullref());
    data_tree::actual_one()(pos, PRE_UNARY_OP_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, IDENTIFIER, name);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> bool_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BOOLEAN, value ? "true" : "false");
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> int_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, INTEGER, value);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> float_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FLOATING, value);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> call::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FUNC_CALL_BEGIN, name);
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& expr)
                  {
                      data_tree::actual_one()(pos, ARGUMENT);
                      expr->compile(nullref());
                  });
    data_tree::actual_one()(pos, FUNC_CALL_END);
    return std::move(nullptr());;
}

util::sptr<proto::expr_base const> func_reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, IDENTIFIER, name + '@' + util::str(param_count));
    return std::move(nullptr());;
}

void clause_builder::add_arith(int indent_level, util::sptr<expr_base const> arith)
{
    data_tree::actual_one()(arith->pos, indent_level, ARITHMETICS);
    util::sptr<expr_base const>(std::move(arith))->compile(nullref());
}

void clause_builder::add_var_def(int indent_level, std::string const& name, util::sptr<expr_base const> init)
{
    data_tree::actual_one()(init->pos, indent_level, VAR_DEF, name);
    util::sptr<expr_base const>(std::move(init))->compile(nullref());
}

void clause_builder::add_return(int indent_level, util::sptr<expr_base const> ret_val)
{
    data_tree::actual_one()(ret_val->pos, indent_level, RETURN);
    util::sptr<expr_base const>(std::move(ret_val))->compile(nullref());
}

void clause_builder::add_return_nothing(int indent_level, misc::pos_type const& pos)
{
    data_tree::actual_one()(pos, indent_level, RETURN, "");
}

void clause_builder::add_function(int indent_level
                                , misc::pos_type const& pos
                                , std::string const& name
                                , std::vector<std::string> const& params)
{
    data_tree::actual_one()(pos, indent_level, FUNC_DEF_HEAD_BEGIN, name);
    std::for_each(params.begin()
                , params.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, indent_level, IDENTIFIER, param);
                  });
    data_tree::actual_one()(pos, indent_level, FUNC_DEF_HEAD_END);
}

void clause_builder::add_if(int indent_level, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    data_tree::actual_one()(pos, indent_level, BRANCH_IF)(pos, indent_level, CONDITION_BEGIN);
    util::sptr<expr_base const>(std::move(condition))->compile(nullref());
    data_tree::actual_one()(pos, indent_level, CONDITION_END);
}

void clause_builder::add_ifnot(int indent_level, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    data_tree::actual_one()(pos, indent_level, BRANCH_IFNOT)(pos, indent_level, CONDITION_BEGIN);
    util::sptr<expr_base const>(std::move(condition))->compile(nullref());
    data_tree::actual_one()(pos, indent_level, CONDITION_END);
}

void clause_builder::add_else(int indent_level, misc::pos_type const& pos)
{
    data_tree::actual_one()(pos, indent_level, BRANCH_ELSE);
}

flchk::block clause_builder::build_and_clear()
{
    return std::move(flchk::block());
}

acceptor_stack::acceptor_stack()
    : _packer(NULL)
{}
