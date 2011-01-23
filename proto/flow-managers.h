#ifndef __STACKENING_PROTO_FLOW_MANAGERS_H__
#define __STACKENING_PROTO_FLOW_MANAGERS_H__

#include <list>

#include "node-base.h"
#include "block.h"
#include "../util/pointer.h"

namespace proto {

    enum termination_status {
        NO_EXPLICIT_TERMINATION,
        RETURN_VOID,
        RETURN_NO_VOID,
    };

    struct flow_mgr_base {
        virtual void add_stmt(util::sptr<stmt_base const> stmt) = 0;
        virtual termination_status termination() const = 0;
    };

    struct flow_mgr
        : public flow_mgr_base
    {
        explicit flow_mgr(block& block_ref)
            : _block_ref(block_ref)
        {}

        void add_stmt(util::sptr<stmt_base const> stmt);
        termination_status termination() const;
    private:
        block& _block_ref;
    };

    struct teminated_flow
        : public flow_mgr_base
    {
        explicit teminated_flow(termination_status status, util::sptr<flow_mgr_base>& host_mgr)
            : term_status(status)
            , _host_mgr(host_mgr)
        {}

        void add_stmt(util::sptr<stmt_base const>);
        termination_status termination() const;

        termination_status const term_status;
    private:
        util::sptr<flow_mgr_base>& _host_mgr;
    };

    struct dummy_flow
        : public flow_mgr_base
    {
        void add_stmt(util::sptr<stmt_base const>) {}

        termination_status termination() const;
    };

}

#endif /* __STACKENING_PROTO_FLOW_MANAGERS_H__ */
