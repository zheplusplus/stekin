#include <gtest/gtest.h>

#include "test-common.h"
#include "phony-err-report.h"
#include "../scope.h"
#include "../symbol-table.h"
#include "../func-templ.h"

using namespace test;

struct FuncNCallTest
    : public proto_test
{
    void SetUp()
    {
        misc::pos_type pos(65535);
        proto_test::SetUp();
        reset_func(pos, "f", std::vector<std::string>());
        inst_scope.reset(new phony_func);
    }

    void reset_func(misc::pos_type const& pos, std::string const& name, std::vector<std::string> const& params)
    {
        func.reset(new proto::func_templ(pos, name, params));
    }

    util::sptr<proto::func_templ> func;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(FuncNCallTest, NoBranchFunc)
{
    misc::pos_type pos(1);
    func->get_scope()->add_func_ret_nothing(pos);
    func->inst(pos, *inst_scope, std::vector<inst::type const*>());
    ASSERT_FALSE(proto::has_error());

    data_tree::expect_one()
        (INIT_AS_VOID_RET)
        (ADD_PATH)
        (NEXT_PATH)
        (pos, SET_RETURN_TYPE_VOID)
        (ADD_STMT)
    ;
}
