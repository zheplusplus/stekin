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

        struct node_type {
            std::string const type_img;

            explicit node_type(std::string const& name)
                : type_img(name)
            {}

            bool operator==(node_type const& rhs) const
            {
                return type_img == rhs.type_img;
            }
        };

        node_type const type_img;

        static node_type const BOOLEAN;
        static node_type const INTEGER;
        static node_type const FLOATING;
        static node_type const IDENTIFIER;

        static node_type const BINARY_OP_BEGIN;
        static node_type const BINARY_OP_END;
        static node_type const PRE_UNARY_OP_BEGIN;
        static node_type const PRE_UNARY_OP_END;
        static node_type const OPERAND;

        static node_type const FUNC_CALL_BEGIN;
        static node_type const FUNC_CALL_END;
        static node_type const ARGUMENT;

        static node_type const VAR_DEF;
        static node_type const ARITHMETICS;
        static node_type const RETURN;

        static node_type const FUNC_DEF_HEAD_BEGIN;
        static node_type const FUNC_DEF_HEAD_END;

        static node_type const CONDITION_BEGIN;
        static node_type const CONDITION_END;

        static node_type const BRANCH_IF;
        static node_type const BRANCH_IFNOT;
        static node_type const BRANCH_ELSE;

        static node_type const LOOP_WHILE;
    public:
        virtual ~data_node_base() {}

        bool operator==(data_node_base const& rhs) const;

        virtual std::ostream& print(std::ostream& os) const = 0;

        virtual bool cmp(data_node_base const& rhs) const = 0;

        virtual bool cmp_no_data(nothing_node const&) const;
        virtual bool cmp_str_data(string_node const&) const;
    protected:
        data_node_base(misc::pos_type const& ps, node_type const& tid)
            : pos(ps)
            , indent_level(-1)
            , type_img(tid)
        {}

        data_node_base(misc::pos_type const& ps, int ind_lvl, node_type const& tid)
            : pos(ps)
            , indent_level(ind_lvl)
            , type_img(tid)
        {}
    };

    struct nothing_node
        : public data_node_base
    {
        nothing_node(misc::pos_type const& pos, node_type const& type_img)
            : data_node_base(pos, type_img)
        {}

        nothing_node(misc::pos_type const& pos, int indent_level, node_type const& type_img)
            : data_node_base(pos, indent_level, type_img)
        {}

        std::ostream& print(std::ostream& os) const;

        bool cmp(data_node_base const& rhs) const;

        bool cmp_no_data(nothing_node const&) const;
    };

    struct string_node
        : public data_node_base
    {
        std::string const data;

        string_node(misc::pos_type const& pos, node_type type_img, std::string const& d)
            : data_node_base(pos, type_img)
            , data(d)
        {}

        string_node(misc::pos_type const& pos, int ind_level, node_type const& type_img, std::string const& d)
            : data_node_base(pos, ind_level, type_img)
            , data(d)
        {}

        std::ostream& print(std::ostream& os) const;

        bool cmp(data_node_base const& rhs) const;

        bool cmp_str_data(string_node const& lhs) const;
    };

}

std::ostream& operator<<(std::ostream& os, test::data_node_base const& node);

#endif /* __STACKENING_PARSER_TEST_DATA_NODE_H__ */
