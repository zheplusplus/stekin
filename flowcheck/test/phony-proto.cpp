#include <algorithm>
#include <vector>
#include <map>

#include "test-common.h"
#include "../../instance/inst-mediate.h"

using namespace test;
using namespace proto;

namespace {

    util::sref<symbol_table> nullsymbols()
    {
        return util::sref<symbol_table>(0);
    }

    std::list<util::sptr<function>> func_entities;

    util::sptr<expr_base const> nullptr()
    {
        return std::move(util::sptr<expr_base const>(0));
    }

    util::sptr<scope> mkscope()
    {
        return std::move(util::mkmptr(new global_scope));
    }

    struct dummy_stmt
        : public stmt_base
    {
        util::sptr<inst::mediate_base> inst(util::sref<inst::scope>) const
        {
            return std::move(util::sptr<inst::mediate_base>(NULL));
        }

        termination_status termination() const
        {
            return NO_EXPLICIT_TERMINATION;
        }
    };

    std::vector<util::sptr<scope>> func_scope_entities;
    symbol_table phony_symbols;

}

void block::add_stmt(util::sptr<stmt_base const>) {}

util::sptr<inst::mediate_base> block::inst(util::sref<inst::scope>) const
{
    return std::move(util::sptr<inst::mediate_base>(NULL));
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

util::sptr<expr_base const> general_scope::make_ref(misc::pos_type const& pos, std::string const& var_name)
{
    data_tree::actual_one()(pos, REFERENCE, var_name);
    return std::move(nullptr());
}

util::sptr<expr_base const> general_scope::make_call(misc::pos_type const& pos
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

util::sptr<expr_base const> scope::make_nega(misc::pos_type const& pos, util::sptr<expr_base const>) const
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
                     , util::sptr<scope>
                     , util::sptr<scope>)
{
    data_tree::actual_one()(pos, BRANCH);
}

void general_scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const>)
{
    data_tree::actual_one()(pos, VAR_DEF, name);
}

util::sptr<scope> general_scope::create_branch_scope()
{
    return std::move(mkscope());
}

util::sref<function> general_scope::decl_func(misc::pos_type const& pos
                                            , std::string const& name
                                            , std::vector<std::string> const& param_names)
{
    data_tree::actual_one()(pos, FUNC_DECL, name);
    std::for_each(param_names.begin()
                , param_names.end()
                , [&](std::string const& param)
                  {
                      data_tree::actual_one()(pos, PARAMETER, param);
                  });
    func_entities.push_back(std::move(util::mkmptr(new function(pos
                                                              , name
                                                              , param_names
                                                              , util::mkref(phony_symbols)))));
    return *func_entities.back();
}

global_scope::global_scope()
{
    data_tree::actual_one()(SCOPE);
}

function::function(misc::pos_type const& p
                 , std::string const&
                 , std::vector<std::string> const&
                 , util::sref<symbol_table const>)
    : pos(p)
{
    func_scope_entities.push_back(std::move(mkscope()));
}
