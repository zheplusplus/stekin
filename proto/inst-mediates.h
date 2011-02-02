#ifndef __STACKENING_PROTO_INSTANTIATE_MEDIATE_H__
#define __STACKENING_PROTO_INSTANTIATE_MEDIATE_H__

#include <list>

#include "node-base.h"
#include "block.h"
#include "../instance/inst-mediate.h"
#include "../util/pointer.h"

namespace proto {

    struct block_mediate
        : public inst::mediate_base
    {
        block_mediate(std::list<util::sptr<stmt_base const>> const& stmts, util::sref<inst::scope> sc);

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope> sc);
        void mediate_inst(util::sref<inst::scope> sc);
    private:
        std::list<util::sptr<stmt_base const>> const& _stmts;
        util::sptr<std::list<util::sptr<inst::mediate_base>>> _mediates;
        util::sptr<inst::block> _inst_block;
    };

    struct direct_inst
        : public inst::mediate_base
    {
        explicit direct_inst(util::sptr<inst::stmt_base const> stmt)
            : _stmt(std::move(stmt))
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope>);
        void mediate_inst(util::sref<inst::scope>);
    private:
        util::sptr<inst::stmt_base const> _stmt;
    };

    struct branch_mediate
        : public inst::mediate_base
    {
        branch_mediate(misc::pos_type const& ps
                     , util::sptr<inst::expr_base const> condition
                     , std::list<util::sptr<stmt_base const>> const& valid_stmts
                     , std::list<util::sptr<stmt_base const>> const& invalid_stmts
                     , util::sref<inst::scope> sc)
            : pos(ps)
            , _condition(std::move(condition))
            , _valid_mediate(valid_stmts, sc)
            , _invalid_mediate(invalid_stmts, sc)
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope> sc);
        void mediate_inst(util::sref<inst::scope> sc);
    public:
        misc::pos_type const pos;
    private:
        util::sptr<inst::expr_base const> _condition;
        block_mediate _valid_mediate;
        block_mediate _invalid_mediate;
    };

    struct loop_mediate
        : public inst::mediate_base
    {
        loop_mediate(misc::pos_type const& ps
                   , util::sptr<inst::expr_base const> condition
                   , std::list<util::sptr<stmt_base const>> const& body_stmts
                   , util::sref<inst::scope> sc)
            : pos(ps)
            , _condition(std::move(condition))
            , _body_mediate(body_stmts, sc)
        {}

        util::sptr<inst::stmt_base const> inst(util::sref<inst::scope> sc);
        void mediate_inst(util::sref<inst::scope> sc);
    public:
        misc::pos_type const pos;
    private:
        util::sptr<inst::expr_base const> _condition;
        block_mediate _body_mediate;
    };

}

#endif /* __STACKENING_PROTO_INSTANTIATE_MEDIATE_H__ */
