#ifndef __STACKENING_PARSER_TEST_DATA_NODE_H__
#define __STACKENING_PARSER_TEST_DATA_NODE_H__

#include <string>
#include <ostream>

#include "../../misc/pos-type.h"

namespace test {

    struct nothing_node;
    struct string_node;

    struct data_node_base {
        misc::pos_type const pos;
        int const indent_level;

        enum node_type {
            INTEGER,
            FLOATING,
            IDENTIFIER,

            BINARY_OP_BEGIN,
            BINARY_OP_END,
            PRE_UNARY_OP_BEGIN,
            PRE_UNARY_OP_END,
            OPERAND,

            FUNC_CALL_BEGIN,
            FUNC_CALL_END,
            ARGUMENT,

            VAR_DEF,
            RETURN,

            FUNC_DEF_HEAD_BEGIN,
            FUNC_DEF_HEAD_END,

            CONDITION_BEGIN,
            CONDITION_END,

            BRANCH_IF,
            BRANCH_IFNOT,
            BRANCH_ELSE,

            LOOP_WHILE,
        };
        node_type const type_id;

        virtual ~data_node_base() {}

        bool operator==(data_node_base const& rhs) const;

        virtual std::ostream& print(std::ostream& os) const;

        virtual bool cmp(data_node_base const& rhs) const = 0;

        virtual bool cmp_no_data(nothing_node const&) const;
        virtual bool cmp_str_data(string_node const&) const;
    protected:
        data_node_base(misc::pos_type const& ps, node_type tid)
            : pos(ps)
            , indent_level(-1)
            , type_id(tid)
        {}

        data_node_base(misc::pos_type const& ps, int ind_lvl, node_type tid)
            : pos(ps)
            , indent_level(ind_lvl)
            , type_id(tid)
        {}
    };

    struct nothing_node
        : public data_node_base
    {
        nothing_node(misc::pos_type const& pos, node_type type_id)
            : data_node_base(pos, type_id)
        {}

        nothing_node(misc::pos_type const& pos, int indent_level, node_type type_id)
            : data_node_base(pos, indent_level, type_id)
        {}

        std::ostream& print(std::ostream& os) const;

        bool cmp(data_node_base const& rhs) const;

        bool cmp_no_data(nothing_node const&) const;
    };

    struct string_node
        : public data_node_base
    {
        std::string const data;

        string_node(misc::pos_type const& pos, node_type type_id, std::string const& d)
            : data_node_base(pos, type_id)
            , data(d)
        {}

        string_node(misc::pos_type const& pos, int indent_level, node_type type_id, std::string const& d)
            : data_node_base(pos, indent_level, type_id)
            , data(d)
        {}

        std::ostream& print(std::ostream& os) const;

        bool cmp(data_node_base const& rhs) const;

        bool cmp_str_data(string_node const& lhs) const;
    };

}

std::ostream& operator<<(std::ostream& os, test::data_node_base const& node);

#endif /* __STACKENING_PARSER_TEST_DATA_NODE_H__ */
