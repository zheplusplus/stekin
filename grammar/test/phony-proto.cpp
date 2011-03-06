#include <algorithm>
#include <vector>
#include <map>

#include "test-common.h"
#include "../../proto/scope.h"
#include "../../proto/func-templ.h"

using namespace test;
using namespace proto;

namespace {

    util::sref<symbol_table> nullsymbols()
    {
        return util::sref<symbol_table>(0);
    }

    struct phony_scope
        : public scope
    {
        phony_scope()
            : scope(nullsymbols())
        {
            data_tree::actual_one()(SCOPE);
        }
    };

    std::list<util::sptr<func_templ>> func_entities;

    util::sptr<expr_base const> nullptr()
    {
        return std::move(util::sptr<expr_base const>(0));
    }

    util::sptr<scope> mkscope()
    {
        return std::move(util::mkmptr(new phony_scope));
    }

    struct dummy_stmt
        : public stmt_base
    {
        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> scope) const
        {
            return std::move(util::sptr<inst::mediate_base>(NULL));
        }

        termination_status termination() const
        {
            return NO_EXPLICIT_TERMINATION;
        }
    };

    std::vector<util::sptr<scope>> func_scope_entities;
    std::map<func_templ const*, std::vector<util::sptr<scope>>::size_type> map_func_to_index;

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
                     , util::sptr<scope>
                     , util::sptr<scope>)
{
    data_tree::actual_one()(pos, BRANCH);
}

void scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    data_tree::actual_one()(pos, VAR_DEF, name);
}

util::sptr<scope> scope::create_branch_scope()
{
    return std::move(mkscope());
}

util::sref<func_templ> scope::decl_func(misc::pos_type const& pos
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
    func_entities.push_back(std::move(util::mkmptr(new func_templ(pos, name, param_names))));
    return *func_entities.back();
}

util::sref<symbol_table> scope::get_symbols() const
{
    return _symbols;
}

util::sptr<scope> scope::global_scope()
{
    return std::move(mkscope());
}

func_templ::func_templ(misc::pos_type const& p, std::string const&, std::vector<std::string> const&)
    : pos(p)
    , _body_scope(nullsymbols())
{
    map_func_to_index[this] = func_scope_entities.size();
    func_scope_entities.push_back(std::move(mkscope()));
}

util::sref<scope> func_templ::get_scope()
{
    return *func_scope_entities[map_func_to_index[this]];
}
