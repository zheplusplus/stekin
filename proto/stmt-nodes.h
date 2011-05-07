#ifndef __STEKIN_PROTO_STATEMENT_NODES_H__
#define __STEKIN_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct BranchMediate
        : public Statement
    {
        BranchMediate(misc::position const& pos
                    , util::sptr<Expression> p
                    , util::sptr<inst::MediateBase> consequence_mediate
                    , util::sptr<inst::MediateBase> alternative_mediate)
            : Statement(pos)
            , predicate(std::move(p))
            , _consequence_mediate(std::move(consequence_mediate))
            , _alternative_mediate(std::move(alternative_mediate))
        {}

        void addTo(util::sref<inst::Scope> scope);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> scope
                                             , util::sref<inst::SymbolTable> st);
        void mediateInst(util::sref<inst::Scope> scope, util::sref<inst::SymbolTable>);
    public:
        util::sptr<Expression> const predicate;
    private:
        util::sptr<inst::MediateBase> const _consequence_mediate;
        util::sptr<inst::MediateBase> const _alternative_mediate;
    };

    struct DirectInst
        : public Statement
    {
        void addTo(util::sref<inst::Scope>);
        util::sptr<inst::Statement const> inst(util::sref<inst::Scope> scope
                                             , util::sref<inst::SymbolTable> st);
        void mediateInst(util::sref<inst::Scope> scope, util::sref<inst::SymbolTable> st);
    protected:
        explicit DirectInst(misc::position const& pos)
            : Statement(pos)
            , _result_stmt_or_nul_if_not_inst(NULL)
        {}

        virtual util::sptr<inst::Statement const> _inst(util::sref<inst::Scope> scope
                                                      , util::sref<inst::SymbolTable> st) const = 0;
    private:
        util::sptr<inst::Statement const> _result_stmt_or_nul_if_not_inst;
    };

    struct Arithmetics
        : public DirectInst
    {
        Arithmetics(misc::position const& pos, util::sptr<Expression> e)
            : DirectInst(pos)
            , expr(std::move(e))
        {}

        util::sptr<Expression> const expr;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<inst::Scope> scope
                                              , util::sref<inst::SymbolTable> st) const;
    };

    struct VarDef
        : public DirectInst
    {
        VarDef(misc::position const& pos, std::string const& n, util::sptr<Expression> i)
            : DirectInst(pos)
            , name(n)
            , init(std::move(i))
        {}

        std::string const name;
        util::sptr<Expression> const init;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<inst::Scope> scope
                                              , util::sref<inst::SymbolTable> st) const;
    };

    struct Return
        : public DirectInst
    {
        Return(misc::position const& pos, util::sptr<Expression> r)
            : DirectInst(pos)
            , ret_val(std::move(r))
        {}

        util::sptr<Expression> const ret_val;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<inst::Scope> scope
                                              , util::sref<inst::SymbolTable> st) const;
    };

    struct ReturnNothing
        : public DirectInst
    {
        explicit ReturnNothing(misc::position const& pos)
            : DirectInst(pos)
        {}
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<inst::Scope> scope
                                              , util::sref<inst::SymbolTable>) const;
    };

}

#endif /* __STEKIN_PROTO_STATEMENT_NODES_H__ */
