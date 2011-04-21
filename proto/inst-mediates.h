#ifndef __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__
#define __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__

#include <list>

#include "node-base.h"
#include "block.h"
#include "../instance/inst-mediate.h"
#include "../util/pointer.h"

namespace proto {

    struct block_mediate
        : public inst::mediate_base
    {
        block_mediate(std::list<util::sptr<Statement const>> const& stmts, util::sref<inst::scope> sc);

        util::sptr<inst::Statement const> inst(util::sref<inst::scope> sc);
        void mediate_inst(util::sref<inst::scope> sc);
    private:
        std::list<util::sptr<Statement const>> const& _stmts;
        util::sptr<std::list<util::sptr<inst::mediate_base>>> _mediates;
        util::sptr<inst::block> _inst_block;
    };

    struct direct_inst
        : public inst::mediate_base
    {
        explicit direct_inst(util::sptr<inst::Statement const> stmt)
            : _stmt(std::move(stmt))
        {}

        util::sptr<inst::Statement const> inst(util::sref<inst::scope>);
        void mediate_inst(util::sref<inst::scope>);
    private:
        util::sptr<inst::Statement const> _stmt;
    };

    struct branch_mediate
        : public inst::mediate_base
    {
        branch_mediate(misc::pos_type const& ps
                     , util::sptr<inst::Expression const> predicate
                     , std::list<util::sptr<Statement const>> const& consequence_stmts
                     , std::list<util::sptr<Statement const>> const& alternative_stmts
                     , util::sref<inst::scope> sc)
            : pos(ps)
            , _predicate(std::move(predicate))
            , _consequence_mediate(consequence_stmts, sc)
            , _alternative_mediate(alternative_stmts, sc)
        {}

        util::sptr<inst::Statement const> inst(util::sref<inst::scope> sc);
        void mediate_inst(util::sref<inst::scope> sc);
    public:
        misc::pos_type const pos;
    private:
        util::sptr<inst::Expression const> _predicate;
        block_mediate _consequence_mediate;
        block_mediate _alternative_mediate;
    };

}

#endif /* __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__ */
