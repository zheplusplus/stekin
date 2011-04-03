#include <algorithm>
#include <sstream>

#include "test-common.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/expr-nodes.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"

using namespace test;
using namespace flchk;

static util::sref<proto::scope> nulscope(NULL);

static util::sptr<proto::expr_base const> nul_proto_expr()
{
    return std::move(util::sptr<proto::expr_base const>(NULL));
}

static util::sptr<flchk::expr_base const> nul_flchk_expr()
{
    return std::move(util::sptr<flchk::expr_base const>(NULL));
}

void filter::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    ret_val->compile(nulscope);
    data_tree::actual_one()(pos, RETURN);
}

void filter::add_func_ret_nothing(misc::pos_type const& pos)
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
}

void filter::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> arith)
{
    arith->compile(nulscope);
    data_tree::actual_one()(pos, ARITHMETICS);
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter>
                      , util::sptr<filter>)
{
    predicate->compile(nulscope);
    data_tree::actual_one()(pos, BRANCH);
}

void filter::add_branch(misc::pos_type const& pos, util::sptr<expr_base const> predicate, util::sptr<filter>)
{
    predicate->compile(nulscope);
    data_tree::actual_one()(pos, BRANCH_CONSQ_ONLY);
}

void filter::add_branch_alt_only(misc::pos_type const& pos
                               , util::sptr<expr_base const> predicate
                               , util::sptr<filter>)
{
    predicate->compile(nulscope);
    data_tree::actual_one()(pos, BRANCH_ALTER_ONLY);
}

void filter::def_var(misc::pos_type const& pos
                   , std::string const& name
                   , util::sptr<expr_base const> init)
{
    init->compile(nulscope);
    data_tree::actual_one()(pos, VAR_DEF, name);
}

void filter::def_func(misc::pos_type const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , util::sptr<filter>)
{
    data_tree::actual_one()(pos, FUNC_DEF, name);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, PARAMETER, param);
                  });
}

block filter::deliver()
{
    return std::move(block());
}

void symbol_def_filter::def_var(misc::pos_type const& pos
                              , std::string const& name
                              , util::sptr<expr_base const> init)
{
    init->compile(nulscope);
    data_tree::actual_one()(pos, VAR_DEF_FILTERED, name);
}

void symbol_def_filter::def_func(misc::pos_type const& pos
                               , std::string const& name
                               , std::vector<std::string> const& param_names
                               , util::sptr<filter>)
{
    data_tree::actual_one()(pos, FUNC_DEF_FILTERED, name);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, PARAMETER, param);
                  });
}

util::sptr<proto::expr_base const> pre_unary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> binary_op::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, op_img);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> conjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "&&");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> disjunction::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "||");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> negation::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, "!");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, REFERENCE, name);
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> bool_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, BOOLEAN, value ? "true" : "false");
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> int_literal::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, INTEGER, value.get_str());
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> float_literal::compile(util::sref<proto::scope>) const
{
    std::stringstream ss;
    ss << value;
    data_tree::actual_one()(pos, FLOATING, ss.str());
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> call::compile(util::sref<proto::scope> s) const
{
    data_tree::actual_one()(pos, CALL, name, args.size());
    std::for_each(args.begin()
                , args.end()
                , [&](util::sptr<expr_base const> const& arg)
                  {
                      arg->compile(s);
                  });
    return std::move(nul_proto_expr());
}

util::sptr<proto::expr_base const> func_reference::compile(util::sref<proto::scope>) const
{
    data_tree::actual_one()(pos, FUNC_REFERENCE, name, param_count);
    return std::move(nul_proto_expr());
}

bool expr_base::is_literal() const
{
    return false;
}

bool expr_base::bool_value() const
{
    return false;
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&
                                             , std::string const&
                                             , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&
                                             , std::string const&
                                             , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const&
                                            , std::string const&
                                            , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> expr_base::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool pre_unary_op::is_literal() const
{
    return false;
}

bool pre_unary_op::bool_value() const
{
    return false;
}

std::string pre_unary_op::type_name() const
{
    return "";
}

util::sptr<expr_base const> pre_unary_op::fold() const
{
    return std::move(nul_flchk_expr());
}

bool binary_op::is_literal() const
{
    return false;
}

bool binary_op::bool_value() const
{
    return false;
}

std::string binary_op::type_name() const
{
    return "";
}

util::sptr<expr_base const> binary_op::fold() const
{
    return std::move(nul_flchk_expr());
}

bool conjunction::is_literal() const
{
    return false;
}

bool conjunction::bool_value() const
{
    return false;
}

std::string conjunction::type_name() const
{
    return "";
}

util::sptr<expr_base const> conjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool disjunction::is_literal() const
{
    return false;
}

bool disjunction::bool_value() const
{
    return false;
}

std::string disjunction::type_name() const
{
    return "";
}

util::sptr<expr_base const> disjunction::fold() const
{
    return std::move(nul_flchk_expr());
}

bool negation::is_literal() const
{
    return false;
}

bool negation::bool_value() const
{
    return false;
}

std::string negation::type_name() const
{
    return "";
}

util::sptr<expr_base const> negation::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string reference::type_name() const
{
    return "";
}

util::sptr<expr_base const> reference::fold() const
{
    return std::move(nul_flchk_expr());
}

bool bool_literal::is_literal() const
{
    return false;
}

bool bool_literal::bool_value() const
{
    return false;
}

std::string bool_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> bool_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&
                                                , std::string const&
                                                , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&
                                                , std::string const&
                                                , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::as_rhs(misc::pos_type const&
                                               , std::string const&
                                               , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> bool_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool int_literal::is_literal() const
{
    return false;
}

bool int_literal::bool_value() const
{
    return false;
}

std::string int_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> int_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&
                                               , std::string const&
                                               , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&
                                               , std::string const&
                                               , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::as_rhs(misc::pos_type const&
                                              , std::string const&
                                              , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> int_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

bool float_literal::is_literal() const
{
    return false;
}

bool float_literal::bool_value() const
{
    return false;
}

std::string float_literal::type_name() const
{
    return "";
}

util::sptr<expr_base const> float_literal::fold() const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&
                                                 , std::string const&
                                                 , mpz_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&
                                                 , std::string const&
                                                 , mpf_class const&) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::operate(misc::pos_type const&, std::string const&, bool) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::as_rhs(misc::pos_type const&
                                                , std::string const&
                                                , util::sptr<expr_base const>) const
{
    return std::move(nul_flchk_expr());
}

util::sptr<expr_base const> float_literal::as_rhs(misc::pos_type const&, std::string const&) const
{
    return std::move(nul_flchk_expr());
}

std::string call::type_name() const
{
    return "";
}

util::sptr<expr_base const> call::fold() const
{
    return std::move(nul_flchk_expr());
}

std::string func_reference::type_name() const
{
    return "";
}

util::sptr<expr_base const> func_reference::fold() const
{
    return std::move(nul_flchk_expr());
}
