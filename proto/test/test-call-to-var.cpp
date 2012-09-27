#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../variable.h"
#include "../type.h"
#include "../function.h"
#include "../func-reference-type.h"

using namespace test;

typedef ProtoTest CallToVarTest;

TEST_F(CallToVarTest, VarNotCallable)
{
    misc::position pos(1);
    misc::position call_pos(100);
    misc::trace trace;
    trace.add(pos);
    proto::Variable var(pos, proto::Type::s_void(), 0, 0);
    var.call(std::vector<util::sref<proto::Type const>>(), trace.add(call_pos));
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
    misc::trace trace;
    trace.add(pos);

    proto::Function func(pos, "shinto_shrine", std::vector<std::string>(), true);
    proto::FuncReferenceType type(pos
                                , util::mkref(func)
                                , 0
                                , std::map<std::string, proto::Variable const>());

    proto::Variable var(pos, util::mkref(type), 0, 0);
    var.call(std::vector<util::sref<proto::Type const>>(), trace.add(call_pos));
    ASSERT_FALSE(error::hasError());

    misc::trace trace_err;
    trace_err.add(pos);
    var.call(std::vector<util::sref<proto::Type const>>({ proto::Type::s_int() })
           , trace_err.add(call_pos_err));
    std::vector<VarCallArgCountWrong> recs = getVarCallArgCountWrong();
    ASSERT_EQ(1, recs.size());
    EXPECT_EQ(call_pos_err, recs[0].call_pos);
}
