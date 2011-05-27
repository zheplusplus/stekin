#ifndef __STEKIN_PROTO_TEST_TEST_COMMON_H__
#define __STEKIN_PROTO_TEST_TEST_COMMON_H__

#include "../type.h"
#include "../variable.h"
#include "../../misc/pos-type.h"
#include "../../test/common.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

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

        ProtoData(int arg_size)
            : pos(-1)
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
        DataTree& operator()(NodeType const& type, std::string const& str, int func_arg_size);
    };

    extern NodeType const BLOCK_BEGIN;
    extern NodeType const BLOCK_END;
    extern NodeType const ARITHMETICS;
    extern NodeType const BRANCH;
    extern NodeType const INITIALIZATION;
    extern NodeType const RETURN;
    extern NodeType const RETURN_NOTHING;

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
    extern NodeType const WRITE;

    extern NodeType const FUNC_DECL;
    extern NodeType const FUNC_IMPL;

    struct ProtoTest
        : public testing::Test
    {
        void SetUp();
        void TearDown();
    };

    struct BlockFiller {
        explicit BlockFiller(util::sref<proto::Block> block)
            : _block(block)
        {}

        BlockFiller branch(misc::position const& pos
                         , util::sptr<proto::Expression const> p
                         , util::sptr<proto::Block> c
                         , util::sptr<proto::Block> a) const;
        BlockFiller arith(misc::position const& pos, util::sptr<proto::Expression const> e) const;
        BlockFiller def(misc::position const& pos
                      , std::string const& name
                      , util::sptr<proto::Expression const> e) const;
        BlockFiller ret(misc::position const& pos, util::sptr<proto::Expression const> e) const;
        BlockFiller ret(misc::position const& pos) const;
    private:
        util::sref<proto::Block> const _block;
    };

}

std::ostream& operator<<(std::ostream& os, test::ProtoData const& data);
std::ostream& operator<<(std::ostream& os, proto::Variable const& var);
std::ostream& operator<<(std::ostream& os, util::sref<proto::Type const> type);

#endif /* __STEKIN_PROTO_TEST_TEST_COMMON_H__ */
