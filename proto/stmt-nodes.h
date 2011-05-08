#ifndef __STEKIN_PROTO_STATEMENT_NODES_H__
#define __STEKIN_PROTO_STATEMENT_NODES_H__

#include "node-base.h"
#include "../instance/fwd-decl.h"

namespace proto {

    struct Branch
        : public Statement
    {
        Branch(misc::position const& ps
             , util::sptr<Expression> p
             , util::sptr<Statement> consequence_stmt
             , util::sptr<Statement> alternative_stmt)
                : pos(ps)
                , predicate(std::move(p))
                , _consequence_stmt(std::move(consequence_stmt))
                , _alternative_stmt(std::move(alternative_stmt))
        {}

        void addTo(util::sref<FuncInstDraft> func);
        util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func
                                             , util::sref<SymbolTable> st);
        void mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable>);
    public:
        misc::position const pos;
        util::sptr<Expression> const predicate;
    private:
        util::sptr<Statement> const _consequence_stmt;
        util::sptr<Statement> const _alternative_stmt;
    };

    struct DirectInst
        : public Statement
    {
        void addTo(util::sref<FuncInstDraft>);
        util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func
                                             , util::sref<SymbolTable> st);
        void mediateInst(util::sref<FuncInstDraft> func, util::sref<SymbolTable> st);

        misc::position const pos;
    protected:
        explicit DirectInst(misc::position const& ps)
            : pos(ps)
            , _result_stmt_or_nul_if_not_inst(NULL)
        {}

        virtual util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                                      , util::sref<SymbolTable> st) const = 0;
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
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable> st) const;
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
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable> st) const;
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
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable> st) const;
    };

    struct ReturnNothing
        : public DirectInst
    {
        explicit ReturnNothing(misc::position const& pos)
            : DirectInst(pos)
        {}
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , util::sref<SymbolTable>) const;
    };

}

#endif /* __STEKIN_PROTO_STATEMENT_NODES_H__ */
