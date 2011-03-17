#include <algorithm>

#include "test-common.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/node-base.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"

using namespace test;
using namespace flchk;

static util::sref<proto::scope> nulscope(NULL);

void filter::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    data_tree::actual_one()(pos, RETURN);
    EXPECT_TRUE(bool(ret_val));
}

void filter::add_func_ret_nothing(misc::pos_type const& pos)
{
    data_tree::actual_one()(pos, RETURN_NOTHING);
}

void filter::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    data_tree::actual_one()(pos, ARITHMETICS);
    EXPECT_TRUE(bool(expr));
}

void filter::add_branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> predicate
                      , util::sptr<filter>
                      , util::sptr<filter>)
{
    data_tree::actual_one()(pos, BRANCH);
    EXPECT_TRUE(bool(predicate));
}

void filter::add_branch(misc::pos_type const& pos, util::sptr<expr_base const> predicate, util::sptr<filter>)
{
    data_tree::actual_one()(pos, BRANCH_CONSQ_ONLY);
    EXPECT_TRUE(bool(predicate));
}

void filter::add_branch_alt_only(misc::pos_type const& pos
                               , util::sptr<expr_base const> predicate
                               , util::sptr<filter>)
{
    data_tree::actual_one()(pos, BRANCH_ALTER_ONLY);
    EXPECT_TRUE(bool(predicate));
}

void filter::def_var(misc::pos_type const& pos
                   , std::string const& name
                   , util::sptr<expr_base const> init)
{
    data_tree::actual_one()(pos, VAR_DEF, name);
    if (!bool(init))
        std::cout << ".se" << std::endl;
    EXPECT_TRUE(bool(init));
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
    data_tree::actual_one()(pos, VAR_DEF_FILTERED, name);
    init->compile(nulscope);
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
