#ifndef __STACKENING_FLOW_CHECK_NODE_BASE_H__
#define __STACKENING_FLOW_CHECK_NODE_BASE_H__

#include <string>
#include <vector>
#include <gmpxx.h>

#include "../proto/fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct stmt_base {
        misc::pos_type const pos;

        virtual util::sptr<proto::stmt_base const> compile(util::sref<proto::scope> scope) const = 0;

        virtual ~stmt_base() {}
    protected:
        explicit stmt_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        stmt_base(stmt_base const&) = delete;
    };

    struct expr_base {
        misc::pos_type const pos;
    public:
        virtual util::sptr<proto::expr_base const> compile(util::sref<proto::scope> scope) const = 0;
    public:
        virtual std::string type_name() const = 0;
    public:
        virtual bool is_literal() const;
        virtual bool bool_value() const;
        virtual util::sptr<expr_base const> fold() const = 0;
    public:
        virtual util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                                  , std::string const& op_img
                                                  , mpz_class const& rhs) const;
        virtual util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                                  , std::string const& op_img
                                                  , mpf_class const& rhs) const;
        virtual util::sptr<expr_base const> operate(misc::pos_type const& op_pos
                                                  , std::string const& op_img
                                                  , bool rhs) const;

        virtual util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos
                                                 , std::string const& op_img
                                                 , util::sptr<expr_base const> lhs) const;
        virtual util::sptr<expr_base const> as_rhs(misc::pos_type const& op_pos
                                                 , std::string const& op_img) const;
    public:
        virtual ~expr_base() {}
    protected:
        expr_base(misc::pos_type const& ps)
            : pos(ps)
        {}

        expr_base(expr_base const&) = delete;
    };

}

#endif /* __STACKENING_FLOW_CHECK_NODE_BASE_H__ */
