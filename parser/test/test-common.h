#ifndef __STACKENING_PARSER_TEST_TEST_COMMON_H__
#define __STACKENING_PARSER_TEST_TEST_COMMON_H__

#include "../../misc/pos-type.h"
#include "../../test/data-node.h"
#include "../../test/data-trees.h"

namespace test {

    struct parser_data {
        int const indent_level;
        misc::pos_type const pos;

        parser_data(int level, misc::pos_type const& ps)
            : indent_level(level)
            , pos(ps)
        {}

        explicit parser_data(misc::pos_type const ps)
            : indent_level(-1)
            , pos(ps)
        {}

        bool operator==(parser_data const& rhs) const
        {
            return indent_level == rhs.indent_level && pos == rhs.pos;
        }
    };

    typedef data_node_templ<parser_data> data_node_base;
    typedef nothing_node_templ<parser_data> nothing_node;
    typedef string_node_templ<parser_data> string_node;

    struct data_tree
        : public data_tree_templ<parser_data, data_tree>
    {
        typedef data_tree_templ<parser_data, data_tree> base_type;

        data_tree& operator()(misc::pos_type const& pos, node_type const& type, std::string const& str);
        data_tree& operator()(misc::pos_type const& pos
                             , int indent
                             , node_type const& type
                             , std::string const& str);
        data_tree& operator()(misc::pos_type const& pos, node_type const& type);
        data_tree& operator()(misc::pos_type const& pos, int indent, node_type const& type);
    };

    extern node_type const BOOLEAN;
    extern node_type const INTEGER;
    extern node_type const FLOATING;
    extern node_type const IDENTIFIER;

    extern node_type const BINARY_OP_BEGIN;
    extern node_type const BINARY_OP_END;
    extern node_type const PRE_UNARY_OP_BEGIN;
    extern node_type const PRE_UNARY_OP_END;
    extern node_type const OPERAND;

    extern node_type const FUNC_CALL_BEGIN;
    extern node_type const FUNC_CALL_END;
    extern node_type const ARGUMENT;

    extern node_type const VAR_DEF;
    extern node_type const ARITHMETICS;
    extern node_type const RETURN;

    extern node_type const FUNC_DEF_HEAD_BEGIN;
    extern node_type const FUNC_DEF_HEAD_END;

    extern node_type const CONDITION_BEGIN;
    extern node_type const CONDITION_END;

    extern node_type const BRANCH_IF;
    extern node_type const BRANCH_IFNOT;
    extern node_type const BRANCH_ELSE;

}

std::ostream& operator<<(std::ostream& os, test::parser_data const& data);

#endif /* __STACKENING_PARSER_TEST_TEST_COMMON_H__ */
