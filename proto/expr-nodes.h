#ifndef __STEKIN_PROTO_EXPRESSION_NODES_H__
#define __STEKIN_PROTO_EXPRESSION_NODES_H__

#include <string>
#include <vector>
#include <gmpxx.h>

#include <instance/fwd-decl.h>

#include "node-base.h"
#include "fwd-decl.h"
#include "func-reference-type.h"

namespace proto {

    struct BoolLiteral
        : public Expression
    {
        BoolLiteral(misc::position const& pos, bool v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>, misc::trace&) const;

        bool const value;
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::position const& pos, mpz_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>, misc::trace&) const;

        mpz_class const value;
    };

    struct FloatLiteral
        : public Expression
    {
        FloatLiteral(misc::position const& pos, mpf_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>, misc::trace&) const;

        mpf_class const value;
    };

    struct ListLiteral
        : public Expression
    {
        ListLiteral(misc::position const& pos, std::vector<util::sptr<Expression const>> v)
            : Expression(pos)
            , value(std::move(v))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        std::vector<util::sptr<Expression const>> const value;
    private:
        util::sref<Type const> _memberType(util::sref<SymbolTable const> st
                                         , misc::trace& trace) const;
        util::sref<Type const> _memberTypeAsPipe(util::sref<SymbolTable const> st
                                               , util::sref<ListContext const> lc
                                               , misc::trace& trace) const;
    };

    struct ListElement
        : public Expression
    {
        explicit ListElement(misc::position const& pos)
            : Expression(pos)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const>
                                        , util::sref<ListContext const> lc
                                        , misc::trace&) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const>
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace&) const;
    };

    struct ListIndex
        : public Expression
    {
        explicit ListIndex(misc::position const& pos)
            : Expression(pos)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const>
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const>
                                        , util::sref<ListContext const>
                                        , misc::trace&) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const>
                                                    , util::sref<ListContext const>
                                                    , misc::trace&) const;
    };

    struct Reference
        : public Expression
    {
        Reference(misc::position const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;

        std::string const name;
    };

    struct Call
        : public Expression
    {
        Call(misc::position const& pos
           , util::sref<Function> f
           , std::vector<util::sptr<Expression const>> a)
                : Expression(pos)
                , _func(f)
                , _args(std::move(a))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;
    private:
        util::sref<Function> const _func;
        std::vector<util::sptr<Expression const>> const _args;
    };

    struct MemberCall
        : public Expression
    {
        MemberCall(misc::position const& pos
                 , util::sptr<Expression const> o
                 , std::string const& mc
                 , std::vector<util::sptr<Expression const>> a)
            : Expression(pos)
            , object(std::move(o))
            , member_call(mc)
            , args(std::move(a))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> object;
        std::string const member_call;
        std::vector<util::sptr<Expression const>> const args;
    };

    struct Functor
        : public Expression
    {
        Functor(misc::position const& pos
              , std::string const& n
              , std::vector<util::sptr<Expression const>> a)
            : Expression(pos)
            , name(n)
            , _args(std::move(a))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        std::string const name;
    private:
        std::vector<util::sptr<Expression const>> const _args;
    private:
        util::sref<FuncInstDraft> _mkDraft(util::sref<SymbolTable const> st
                                         , misc::trace& trace) const;
        util::sref<FuncInstDraft> _mkDraftAsPipe(util::sref<SymbolTable const> st
                                               , util::sref<ListContext const> lc
                                               , misc::trace& trace) const;
    };

    struct FuncReference
        : public Expression
    {
        FuncReference(misc::position const& pos, util::sref<Function> f)
            : Expression(pos)
            , _func(f)
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
    private:
        util::sref<Function> const _func;
    };

    struct ListAppend
        : public Expression
    {
        ListAppend(misc::position const& pos
                 , util::sptr<Expression const> l
                 , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(misc::position const& pos
                , util::sptr<Expression const> l
                , std::string const& o
                , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , op(o)
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const lhs;
        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(misc::position const& pos, std::string const& o, util::sptr<Expression const> r)
            : Expression(pos)
            , op(o)
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        std::string const op;
        util::sptr<Expression const> const rhs;
    };

    struct Conjunction
        : public Expression
    {
        Conjunction(misc::position const& pos
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Disjunction
        : public Expression
    {
        Disjunction(misc::position const& pos
                  , util::sptr<Expression const> l
                  , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const lhs;
        util::sptr<Expression const> const rhs;
    };

    struct Negation
        : public Expression
    {
        Negation(misc::position const& pos, util::sptr<Expression const> r)
            : Expression(pos)
            , rhs(std::move(r))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const>, misc::trace&) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace&) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const rhs;
    };

}

#endif /* __STEKIN_PROTO_EXPRESSION_NODES_H__ */
