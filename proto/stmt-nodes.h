#ifndef __STEKIN_PROTO_STATEMENT_NODES_H__
#define __STEKIN_PROTO_STATEMENT_NODES_H__

#include "node-base.h"

namespace proto {

    struct Branch
        : public Statement
    {
        Branch(misc::position const& ps
             , util::sptr<Expression const> p
             , util::sptr<Statement> consequence_stmt
             , util::sptr<Statement> alternative_stmt)
                : pos(ps)
                , predicate(std::move(p))
                , _consequence_stmt(std::move(consequence_stmt))
                , _alternative_stmt(std::move(alternative_stmt))
        {}

        void addTo(util::sref<FuncInstDraft> func);
        util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func, misc::trace& trace);
        void mediateInst(util::sref<FuncInstDraft> func, misc::trace&);
        std::vector<util::sptr<inst::Function const>> deliverFuncs();
    public:
        misc::position const pos;
        util::sptr<Expression const> const predicate;
    private:
        util::sptr<Statement> const _consequence_stmt;
        util::sptr<Statement> const _alternative_stmt;
    };

    struct DirectInst
        : public Statement
    {
        void addTo(util::sref<FuncInstDraft>);
        util::sptr<inst::Statement const> inst(util::sref<FuncInstDraft> func, misc::trace& trace);
        void mediateInst(util::sref<FuncInstDraft> func, misc::trace& trace);
        std::vector<util::sptr<inst::Function const>> deliverFuncs();

        misc::position const pos;
    protected:
        explicit DirectInst(misc::position const& ps)
            : pos(ps)
            , _result_stmt_or_nul_if_not_inst(nullptr)
        {}

        virtual util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                                      , misc::trace& trace) const = 0;
    private:
        util::sptr<inst::Statement const> _result_stmt_or_nul_if_not_inst;
    };

    struct Arithmetics
        : public DirectInst
    {
        Arithmetics(misc::position const& pos, util::sptr<Expression const> e)
            : DirectInst(pos)
            , expr(std::move(e))
        {}

        util::sptr<Expression const> const expr;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const;
    };

    struct VarDef
        : public DirectInst
    {
        VarDef(misc::position const& pos, std::string const& n, util::sptr<Expression const> i)
            : DirectInst(pos)
            , name(n)
            , init(std::move(i))
        {}

        std::string const name;
        util::sptr<Expression const> const init;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const;
    };

    struct Return
        : public DirectInst
    {
        Return(misc::position const& pos, util::sptr<Expression const> r)
            : DirectInst(pos)
            , ret_val(std::move(r))
        {}

        util::sptr<Expression const> const ret_val;
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const;
    };

    struct ReturnNothing
        : public DirectInst
    {
        explicit ReturnNothing(misc::position const& pos)
            : DirectInst(pos)
        {}
    protected:
        util::sptr<inst::Statement const> _inst(util::sref<FuncInstDraft> func
                                              , misc::trace& trace) const;
    };

}

#endif /* __STEKIN_PROTO_STATEMENT_NODES_H__ */
