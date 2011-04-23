#ifndef __STEKIN_PROTO_TEST_TEST_COMMON_H__
#define __STEKIN_PROTO_TEST_TEST_COMMON_H__

#include "../../instance/node-base.h"
#include "../../instance/type.h"
#include "../../instance/function.h"
#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    struct PhonyFunc
        : public inst::Function
    {
        PhonyFunc()
            : Function(0
                     , std::list<inst::ArgNameTypeRec>()
                     , std::map<std::string, inst::Variable const>())
        {}
    };

    extern inst::BuiltInPrimitive const PROTO_TEST_TYPE;
    extern inst::BuiltInPrimitive const PROTO_TEST_VOID;

    struct ProtoData {
        misc::position const pos;
        int const func_arg_size;

        ProtoData()
            : pos(-1)
            , func_arg_size(-1)
        {}

        explicit ProtoData(misc::position const ps)
            : pos(ps)
            , func_arg_size(-1)
        {}

        ProtoData(misc::position const& ps, int arg_size)
            : pos(ps)
            , func_arg_size(arg_size)
        {}

        bool operator==(ProtoData const& rhs) const
        {
            return pos == rhs.pos && func_arg_size == rhs.func_arg_size;
        }
    };

    struct DataTree
        : public DataTreeTempl<ProtoData, DataTree>
    {
        typedef DataTreeTempl<ProtoData, DataTree> BaseType;

        DataTree& operator()(misc::position const& pos
                           , NodeType const& type
                           , std::string const& str);
        DataTree& operator()(misc::position const& pos
                           , NodeType const& type
                           , std::string const& str
                           , int func_arg_size);
        DataTree& operator()(misc::position const& pos, NodeType const& type);

        DataTree& operator()(NodeType const& type);
        DataTree& operator()(NodeType const& type, std::string const& str);
    };

    extern NodeType const ADD_STMT_TO_SCOPE;
    extern NodeType const ADD_STMT_TO_BLOCK;

    extern NodeType const QUERY_VAR;
    extern NodeType const QUERY_BINARY_OP;
    extern NodeType const QUERY_PRE_UNARY_OP;

    extern NodeType const BOOLEAN;
    extern NodeType const INTEGER;
    extern NodeType const FLOATING;
    extern NodeType const REFERENCE;
    extern NodeType const BINARY_OP;
    extern NodeType const PRE_UNARY_OP;
    extern NodeType const CALL;
    extern NodeType const FUNC_REFERENCE;
    extern NodeType const CONJUNCTION;
    extern NodeType const DISJUNCTION;
    extern NodeType const NEGATION;

    extern NodeType const INIT_AS_VOID_RET;
    extern NodeType const SET_RETURN_TYPE_VOID;
    extern NodeType const SET_RETURN_TYPE;
    extern NodeType const QUERY_RETURN_TYPE_RESOLVE_STATUS;

    extern NodeType const NEXT_PATH;
    extern NodeType const ADD_PATH;

    struct ProtoTest
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

}

std::ostream& operator<<(std::ostream& os, test::ProtoData const& data);

#endif /* __STEKIN_PROTO_TEST_TEST_COMMON_H__ */
