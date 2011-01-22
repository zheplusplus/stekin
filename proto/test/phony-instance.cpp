#include "test-common.h"
#include "../../instance/scope.h"
#include "../../util/string.h"

using namespace test;
using namespace inst;

type const* const type::BIT_VOID = &test::PROTO_TEST_TYPE;

void scope::set_return_type(misc::pos_type const& pos, type const* type) const
{
}

variable scope::def_var(misc::pos_type const& pos, type const* vtype, std::string const&) const
{
    return variable(pos, vtype, 0, 0);
}

variable scope::query_var(misc::pos_type const& pos, std::string const& name) const
{
    data_tree::actual_one()(pos, QUERY_VAR, name);
    return variable(misc::pos_type(0), &test::PROTO_TEST_TYPE, 0, 0);
}

operation const* scope::query_binary(misc::pos_type const& pos
                                   , std::string const& op
                                   , type const*
                                   , type const*) const
{
    data_tree::actual_one()(pos, QUERY_BINARY_OP, op);
    return NULL;
}

operation const* scope::query_pre_unary(misc::pos_type const& pos, std::string const& op, type const*) const
{
    data_tree::actual_one()(pos, QUERY_PRE_UNARY_OP, op);
    return NULL;
}

void scope::add_stmt(util::sptr<stmt_base const> stmt)
{
    data_tree::actual_one()(ADD_STMT);
}

int scope::level() const
{
    return 0;
}

symbol_table::symbol_table(
            int, std::list<arg_name_type_pair> const&, std::map<std::string, variable const> const&)
    : level(0)
    , _ss_used(0)
{}

util::sref<function> function::create_instance(std::vector<type const*> const&
                                             , std::map<std::string, variable const> const&
                                             , bool)
{
    return util::sref<function>(NULL);
}

void block::add_stmt(util::sptr<stmt_base const>)
{
}

type const* int_literal::typeof() const
{
    data_tree::actual_one()(INTEGER, util::str(value));
    return type::BIT_VOID;
}

type const* float_literal::typeof() const
{
    data_tree::actual_one()(FLOATING, util::str(value));
    return type::BIT_VOID;
}

type const* bool_literal::typeof() const
{
    data_tree::actual_one()(BOOLEAN, value ? "true" : "false");
    return type::BIT_VOID;
}

type const* reference::typeof() const
{
    data_tree::actual_one()(REFERENCE);
    return type::BIT_VOID;
}

type const* call::typeof() const
{
    data_tree::actual_one()(CALL, util::str(int(args.size())));
    return type::BIT_VOID;
}

type const* binary_op::typeof() const
{
    data_tree::actual_one()(BINARY_OP);
    lhs->typeof();
    rhs->typeof();
    return type::BIT_VOID;
}

type const* pre_unary_op::typeof() const
{
    data_tree::actual_one()(PRE_UNARY_OP);
    rhs->typeof();
    return type::BIT_VOID;
}

type const* conjunction::typeof() const
{
    data_tree::actual_one()(CONJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return type::BIT_VOID;
}

type const* disjunction::typeof() const
{
    data_tree::actual_one()(DISJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return type::BIT_VOID;
}

type const* negation::typeof() const
{
    data_tree::actual_one()(NEGATION);
    rhs->typeof();
    return type::BIT_VOID;
}
