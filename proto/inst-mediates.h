#ifndef __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__
#define __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__

#include <list>

#include "node-base.h"
#include "block.h"
#include "../instance/inst-mediate.h"
#include "../util/pointer.h"

namespace proto {

    struct DirectInst
        : public inst::MediateBase
    {
        explicit DirectInst(util::sptr<inst::Statement const> stmt)
            : _stmt(std::move(stmt))
        {}

        void addTo(util::sref<inst::Scope>);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope>);
        void mediateInst(util::sref<inst::Scope>);
    private:
        util::sptr<inst::Statement const> _stmt;
    };

    struct BlockMediate
        : public inst::MediateBase
    {
        BlockMediate(std::list<util::sptr<Statement const>> const& stmts)
            : _stmts(stmts)
            , _mediates(NULL)
        {}

        void addTo(util::sref<inst::Scope> scope);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> scope);
        void mediateInst(util::sref<inst::Scope> scope);
    private:
        std::list<util::sptr<Statement const>> const& _stmts;
        util::sptr<std::list<util::sptr<inst::MediateBase>>> _mediates;
    };

    struct BranchMediate
        : public inst::MediateBase
    {
        BranchMediate(misc::position const& ps
                    , util::sptr<inst::Expression const> predicate
                    , util::sptr<inst::MediateBase> consequence_mediate
                    , util::sptr<inst::MediateBase> alternative_mediate)
            : pos(ps)
            , _predicate(std::move(predicate))
            , _consequence_mediate(std::move(consequence_mediate))
            , _alternative_mediate(std::move(alternative_mediate))
        {}

        void addTo(util::sref<inst::Scope> scope);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> scope);
        void mediateInst(util::sref<inst::Scope> scope);
    public:
        misc::position const pos;
    private:
        util::sptr<inst::Expression const> _predicate;
        util::sptr<inst::MediateBase> const _consequence_mediate;
        util::sptr<inst::MediateBase> const _alternative_mediate;
    };

}

#endif /* __STEKIN_PROTO_INSTANTIATE_MEDIATE_H__ */
