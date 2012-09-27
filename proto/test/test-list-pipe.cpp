#include <gtest/gtest.h>

#include <test/phony-errors.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../list-pipe.h"
#include "../list-types.h"
#include "../symbol-table.h"

using namespace test;

struct ListPipeTest
    : public ProtoTest
{
    ListPipeTest()
        : global_st(nullptr)
    {}

    void SetUp()
    {
        ProtoTest::SetUp();
        global_st.reset(new proto::SymbolTable);
    }

    util::sptr<proto::SymbolTable> global_st;
};

TEST_F(ListPipeTest, ElementNotInList)
{
    misc::position pos(1);
    misc::trace trace;
    trace.add(pos);

    std::vector<util::sptr<proto::PipeBase const>> pipes;
    pipes.push_back(util::mkptr(new proto::PipeMap(util::mkptr(new proto::ListElement(pos)))));
    pipes.push_back(util::mkptr(new proto::PipeFilter(util::mkptr(new proto::ListElement(pos)))));
    util::sptr<proto::BoolLiteral const> fake_list(new proto::BoolLiteral(pos, false));
    proto::ListPipeline pipeline(pos, std::move(fake_list), std::move(pipes));
    pipeline.type(*global_st, trace);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getPipeNotApplyOnList().size());
    ASSERT_EQ(pos, getPipeNotApplyOnList()[0].pos);
}

TEST_F(ListPipeTest, ListOfElementNotInList)
{
    misc::position pos(2);
    misc::trace trace;
    trace.add(pos);
    proto::ListContext context(util::sref<proto::Type const>(nullptr));

    std::vector<util::sptr<proto::PipeBase const>> pipes;
    pipes.push_back(util::mkptr(new proto::PipeMap(util::mkptr(new proto::ListElement(pos)))));
    pipes.push_back(util::mkptr(new proto::PipeFilter(util::mkptr(new proto::ListElement(pos)))));
    std::vector<util::sptr<proto::Expression const>> ls;
    ls.push_back(util::mkptr(new proto::ListElement(pos)));
    util::sptr<proto::ListLiteral const> fake_list(new proto::ListLiteral(pos, std::move(ls)));
    proto::ListPipeline pipeline(pos, std::move(fake_list), std::move(pipes));
    pipeline.typeAsPipe(*global_st, util::mkref(context), trace);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(1, getPipeNotApplyOnList().size());
    ASSERT_EQ(pos, getPipeNotApplyOnList()[0].pos);
}

TEST_F(ListPipeTest, MapType)
{
    misc::position pos(3);
    misc::trace trace;
    trace.add(pos);
    util::sptr<proto::ListContext const> context(new proto::ListContext(proto::Type::s_float()));

    std::vector<util::sptr<proto::Expression const>> ls;
    ls.push_back(util::mkptr(new proto::IntLiteral(pos, mpz_class(0))));
    util::sptr<proto::ListLiteral const> list(new proto::ListLiteral(pos, std::move(ls)));
    std::vector<util::sptr<proto::PipeBase const>> pipes0;
    pipes0.push_back(util::mkptr(new proto::PipeMap(util::mkptr(new proto::ListElement(pos)))));
    proto::ListPipeline pl0(pos, std::move(list), std::move(pipes0));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_int()), pl0.type(*global_st, trace));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_int())
            , pl0.typeAsPipe(*global_st, *context, trace));
    ASSERT_FALSE(error::hasError());

    ls.push_back(util::mkptr(new proto::ListElement(pos)));
    list.reset(new proto::ListLiteral(pos, std::move(ls)));
    std::vector<util::sptr<proto::PipeBase const>> pipes1;
    util::sptr<proto::Expression const> compare(
            new proto::BinaryOp(pos
                              , util::mkptr(new proto::FloatLiteral(pos, mpf_class(0.0)))
                              , "<"
                              , util::mkptr(new proto::ListElement(pos))));
    pipes1.push_back(util::mkptr(new proto::PipeMap(std::move(compare))));
    proto::ListPipeline pl1(pos, std::move(list), std::move(pipes1));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_bool())
            , pl1.typeAsPipe(*global_st, *context, trace));
    ASSERT_FALSE(error::hasError());
}

TEST_F(ListPipeTest, FilterType)
{
    misc::position pos(4);
    misc::trace trace;
    trace.add(pos);
    util::sptr<proto::ListContext const> context(new proto::ListContext(proto::Type::s_float()));

    std::vector<util::sptr<proto::Expression const>> ls;
    ls.push_back(util::mkptr(new proto::IntLiteral(pos, mpz_class(0))));
    util::sptr<proto::ListLiteral const> list(new proto::ListLiteral(pos, std::move(ls)));
    std::vector<util::sptr<proto::PipeBase const>> pipes0;
    util::sptr<proto::Expression const> cmp0(
            new proto::BinaryOp(pos
                              , util::mkptr(new proto::IntLiteral(pos, mpz_class(0)))
                              , "<"
                              , util::mkptr(new proto::ListElement(pos))));
    pipes0.push_back(util::mkptr(new proto::PipeFilter(std::move(cmp0))));
    proto::ListPipeline pl0(pos, std::move(list), std::move(pipes0));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_int()), pl0.type(*global_st, trace));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_int())
            , pl0.typeAsPipe(*global_st, *context, trace));
    ASSERT_FALSE(error::hasError());

    ls.push_back(util::mkptr(new proto::ListElement(pos)));
    list.reset(new proto::ListLiteral(pos, std::move(ls)));
    std::vector<util::sptr<proto::PipeBase const>> pipes1;
    util::sptr<proto::Expression const> cmp1(
            new proto::BinaryOp(pos
                              , util::mkptr(new proto::FloatLiteral(pos, mpf_class(0.0)))
                              , "<"
                              , util::mkptr(new proto::ListElement(pos))));
    pipes1.push_back(util::mkptr(new proto::PipeFilter(std::move(cmp1))));
    proto::ListPipeline pl1(pos, std::move(list), std::move(pipes1));
    ASSERT_EQ(proto::ListType::getListType(proto::Type::s_float())
            , pl1.typeAsPipe(*global_st, *context, trace));
    ASSERT_FALSE(error::hasError());
}
