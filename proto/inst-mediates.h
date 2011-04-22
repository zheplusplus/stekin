#ifndef __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__
#define __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__

#include <list>

#include "node-base.h"
#include "block.h"
#include "../instance/inst-mediate.h"
#include "../util/pointer.h"

namespace proto {

    struct BlockMediate
        : public inst::MediateBase
    {
        BlockMediate(std::list<util::sptr<Statement const>> const& stmts, util::sref<inst::Scope> sc);

        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> sc);
        void mediate_inst(util::sref<inst::Scope> sc);
    private:
        std::list<util::sptr<Statement const>> const& _stmts;
        util::sptr<std::list<util::sptr<inst::MediateBase>>> _mediates;
        util::sptr<inst::Block> _inst_block;
    };

    struct DirectInst
        : public inst::MediateBase
    {
        explicit DirectInst(util::sptr<inst::Statement const> stmt)
            : _stmt(std::move(stmt))
        {}

        util::sptr<inst::Statement const> inst(util::sref<inst::Scope>);
        void mediate_inst(util::sref<inst::Scope>);
    private:
        util::sptr<inst::Statement const> _stmt;
    };

    struct BranchMediate
        : public inst::MediateBase
    {
        BranchMediate(misc::position const& ps
                     , util::sptr<inst::Expression const> predicate
                     , std::list<util::sptr<Statement const>> const& consequence_stmts
                     , std::list<util::sptr<Statement const>> const& alternative_stmts
                     , util::sref<inst::Scope> sc)
            : pos(ps)
            , _predicate(std::move(predicate))
            , _consequence_mediate(consequence_stmts, sc)
            , _alternative_mediate(alternative_stmts, sc)
        {}

        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> sc);
        void mediate_inst(util::sref<inst::Scope> sc);
    public:
        misc::position const pos;
    private:
        util::sptr<inst::Expression const> _predicate;
        BlockMediate _consequence_mediate;
        BlockMediate _alternative_mediate;
    };

}

#endif /* __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__ */
