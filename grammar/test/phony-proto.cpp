#include <algorithm>
#include <cassert>

#include "test-common.h"
#include "../../proto/scope.h"
#include "../../proto/func-templ.h"

using namespace test;
using namespace proto;

namespace {

    struct phony_scope
        : public scope
    {
        phony_scope()
            : scope(util::sref<symbol_table>(0))
        {
            data_tree::actual_one()(SCOPE);
        }
    };

    static util::sptr<expr_base const> nullptr()
    {
        return std::move(util::sptr<expr_base const>(0));
    }

    static util::sptr<scope> mkscope()
    {
        return std::move(util::mkmptr(new phony_scope));
    }

}

util::sptr<expr_base const> scope::make_bool(misc::pos_type const& pos, bool value) const
{
    data_tree::actual_one()(pos, BOOLEAN, value ? "true" : "false");
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_int(misc::pos_type const& pos, std::string const& value) const
{
    data_tree::actual_one()(pos, INTEGER, value);
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_float(misc::pos_type const& pos, std::string const& value) const
{
    data_tree::actual_one()(pos, FLOATING, value);
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_ref(misc::pos_type const& pos, std::string const& var_name) const
{
    data_tree::actual_one()(pos, REFERENCE, var_name);
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_call(misc::pos_type const& pos
                                           , std::string const& func_name
                                           , std::vector<util::sptr<expr_base const>> args) const
{
    data_tree::actual_one()(pos, CALL, func_name, args.size());
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_binary(misc::pos_type const& pos
                                             , util::sptr<expr_base const>
                                             , std::string const& op
                                             , util::sptr<expr_base const>) const
{
    data_tree::actual_one()(pos, BINARY_OP, op);
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_pre_unary(misc::pos_type const& pos
                                                , std::string const& op
                                                , util::sptr<expr_base const>) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, op);
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_conj(misc::pos_type const& pos
                                           , util::sptr<expr_base const>
                                           , util::sptr<expr_base const>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "&&");
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_disj(misc::pos_type const& pos
                                           , util::sptr<expr_base const>
                                           , util::sptr<expr_base const>) const
{
    data_tree::actual_one()(pos, BINARY_OP, "||");
    return std::move(nullptr());
}

util::sptr<expr_base const> scope::make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const
{
    data_tree::actual_one()(pos, PRE_UNARY_OP, "!");
    return std::move(nullptr());
}

void scope::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const>)
{
    data_tree::actual_one()(pos, RETURN);
}

void scope::add_func_ret_nothing(misc::pos_type const& pos)
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
}

void scope::add_arith(misc::pos_type const& pos, util::sptr<expr_base const>)
{
    data_tree::actual_one()(pos, ARITHMETICS);
}

void scope::add_branch(misc::pos_type const& pos
                     , util::sptr<expr_base const>
                     , util::sptr<stmt_base const>
                     , util::sptr<stmt_base const>)
{
    data_tree::actual_one()(pos, BRANCH);
}

void scope::add_loop(misc::pos_type const& pos, util::sptr<expr_base const>, util::sptr<stmt_base const>)
{
    data_tree::actual_one()(pos, LOOP);
}

void scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    data_tree::actual_one()(pos, VAR_DEF, name);
}

util::sptr<scope> scope::create_branch_scope()
{
    return std::move(mkscope());
}

util::sptr<scope> scope::create_loop_scope()
{
    return std::move(mkscope());
}

util::sref<func_templ> scope::decl_func(misc::pos_type const& pos
                                      , std::string const& name
                                      , std::vector<std::string> const& param_names)
{
    return util::sref<func_templ>(0);
}

util::sptr<stmt_base const> scope::extract_stmts()
{
    return util::sptr<stmt_base const>(0);
}

util::sref<symbol_table> scope::get_symbols() const
{
    return _symbols;
}

util::sptr<scope> scope::global_scope()
{
    return std::move(mkscope());
}

util::sref<scope> func_templ::get_scope()
{
    return util::mkref(_body_scope);
}

void flow_mgr::add_stmt(util::sptr<stmt_base const>) {}

termination_status flow_mgr::termination() const
{
    return NO_EXPLICIT_TERMINATION;
}
