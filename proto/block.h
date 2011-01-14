#ifndef __STAKCENING_PROTO_BLOCK_H__
#define __STAKCENING_PROTO_BLOCK_H__

#include <vector>

#include "node-base.h"
#include "../util/pointer.h"

namespace proto {

    struct block_base {
        virtual void func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val);
        virtual void func_ret_nothing(misc::pos_type const& pos);
        virtual void make_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr);

        virtual void branch(misc::pos_type const& pos
                          , util::sptr<expr_base const> condition
                          , util::sptr<block_base const> valid
                          , util::sptr<block_base const> invalid);
        virtual void loop(misc::pos_type const& pos
                        , util::sptr<expr_base const> condition
                        , util::sptr<block_base const> body);

        virtual void def_var(misc::pos_type const& pos
                           , std::string const& name
                           , util::sptr<expr_base const> init);

        virtual util::sptr<block_base> sub_block() const = 0;
    private:
        std::list<util::sptr<stmt_base const>> _stmts;
    protected:
        struct flow_mgr {
            flow_mgr(std::list<util::sptr<stmt_base const>>& stmts_ref)
                : _stmts_ref(stmt_ref)
            {}

            virtual void add_stmt(util::sptr<stmt_base const> stmt);
            virtual termination_status termination() const;
        private:
            std::list<util::sptr<stmt_base const>>& _stmts_ref;
        };

        util::sptr<flow_mgr> _flow_mgr;

        block_base()
            : _flow_mgr(new flow_mgr(_stmts))
        {}

        block_base(block_base const&) = delete;

        block_base(block_base&& rhs)
            : _stmts(std::move(rhs._stmts))
            , _flow_mgr(std::move(rhs._flow_mgr))
        {}
    };

    struct global_block
        : public block_base
    {
        void func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val);
        void func_ret_nothing(misc::pos_type const& pos);

        util::sptr<block_base> sub_block() const;
    };

    struct global_sub_block
        : public global_block
    {
        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);
    };

    struct func_block
        : public block_base
    {
        util::sptr<block_base> sub_block() const;
    };

    struct func_sub_block
        : public func_block
    {
        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);
    };

}

#endif /* __STAKCENING_PROTO_BLOCK_H__ */
