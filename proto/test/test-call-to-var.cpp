#include <gtest/gtest.h>

#include "test-common.h"
#include "../variable.h"
#include "../type.h"
#include "../function.h"
#include "../func-reference-type.h"
#include "../../test/phony-errors.h"

using namespace test;

struct CallToVarTest
    : public ProtoTest
{
    void SetUp()
    {
        ProtoTest::SetUp();
    }
};

TEST_F(CallToVarTest, VarNotCallable)
{
    misc::position pos(1);
    misc::position call_pos(100);
    proto::Variable var(pos, proto::Type::BIT_VOID, 0, 0);
    var.call(call_pos, std::vector<util::sref<proto::Type const>>());
    ASSERT_TRUE(error::hasError());

    std::vector<VariableNotCallableRec> recs = getVariableNotCallables();
    ASSERT_EQ(1, recs.size());
    EXPECT_EQ(call_pos, recs[0].call_pos);
}

TEST_F(CallToVarTest, ArgCountWrong)
{
    misc::position pos(2);
    misc::position call_pos(200);
    misc::position call_pos_err(201);

    proto::Function func(pos, "shinto_shrine", std::vector<std::string>(), true);
    proto::FuncReferenceType type(pos
                                , util::mkref(func)
                                , 0
                                , std::map<std::string, proto::Variable const>());

    proto::Variable var(pos, util::mkref(type), 0, 0);
    var.call(call_pos, std::vector<util::sref<proto::Type const>>());
    ASSERT_FALSE(error::hasError());

    var.call(call_pos_err, std::vector<util::sref<proto::Type const>>({ proto::Type::BIT_INT }));
    std::vector<VarCallArgCountWrong> recs = getVarCallArgCountWrong();
    ASSERT_EQ(1, recs.size());
    EXPECT_EQ(call_pos_err, recs[0].call_pos);
}
