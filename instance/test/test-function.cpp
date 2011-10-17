#include <gtest/gtest.h>

#include "test-common.h"
#include "../function.h"
#include "../types.h"
#include "../block.h"

using namespace test;

typedef InstanceTest FunctionTest;

TEST_F(FunctionTest, WriteDecl)
{
    inst::Function const func_no_param(util::mkptr(new inst::VoidPrimitive)
                                     , 1
                                     , 0
                                     , std::list<inst::Function::ParamInfo>()
                                     , util::serial_num::next()
                                     , std::vector<int>()
                                     , util::mkptr(new inst::Block));
    func_no_param.writeDecl();

    std::list<inst::Function::ParamInfo> params;
    params.push_back(inst::Function::ParamInfo(util::mkptr(new inst::IntPrimitive)
                                             , inst::Address(0, 0)));
    params.push_back(inst::Function::ParamInfo(util::mkptr(new inst::BoolPrimitive)
                                             , inst::Address(0, 8)));
    inst::Function const func_2_param(util::mkptr(new inst::FloatPrimitive)
                                    , 1
                                    , 0
                                    , std::move(params)
                                    , util::serial_num::next()
                                    , std::vector<int>()
                                    , util::mkptr(new inst::Block));
    func_2_param.writeDecl();

    DataTree::expectOne()
        (FUNC_DECL_BEGIN, "void", 1, 0)
        (FUNC_RES_ENTRY, 0)
        (FUNC_DECL_END)

        (FUNC_DECL_BEGIN, "float", 1, 0)
        (FUNC_RES_ENTRY, 0)
            (PARAMETER, "int", 0, 0)
            (PARAMETER, "bool", 0, 8)
        (FUNC_DECL_END)
    ;
}

TEST_F(FunctionTest, WriteImpl)
{
    inst::Function const func_no_param(util::mkptr(new inst::VoidPrimitive)
                                     , 1
                                     , 0
                                     , std::list<inst::Function::ParamInfo>()
                                     , util::serial_num::next()
                                     , std::vector<int>()
                                     , util::mkptr(new inst::Block));
    func_no_param.writeImpl();

    std::list<inst::Function::ParamInfo> params;
    params.push_back(inst::Function::ParamInfo(util::mkptr(new inst::IntPrimitive)
                                             , inst::Address(0, 0)));
    params.push_back(inst::Function::ParamInfo(util::mkptr(new inst::BoolPrimitive)
                                             , inst::Address(0, 8)));
    inst::Function const func_2_param(util::mkptr(new inst::FloatPrimitive)
                                    , 1
                                    , 0
                                    , std::move(params)
                                    , util::serial_num::next()
                                    , std::vector<int>()
                                    , util::mkptr(new inst::Block));
    func_2_param.writeImpl();

    DataTree::expectOne()
        (FUNC_DEF, "void")
            (BLOCK_BEGIN)
            (BLOCK_END)
        (FUNC_DEF, "float")
            (BLOCK_BEGIN)
            (BLOCK_END)
    ;
}
