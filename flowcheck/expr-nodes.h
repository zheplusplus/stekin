#ifndef __STEKIN_FLOWCHECK_EXPRESSION_NODES_H__
#define __STEKIN_FLOWCHECK_EXPRESSION_NODES_H__

#include <string>
#include <vector>

#include "node-base.h"
#include "fwd-decl.h"
#include "../util/pointer.h"

namespace flchk {

    struct PreUnaryOp
        : public Expression
    {
        PreUnaryOp(misc::position const& pos, std::string const& op, util::sptr<Expression const> r)
            : Expression(pos)
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        bool isLiteral() const;
        bool boolValue() const;
        util::sptr<Expression const> fold() const;
        std::string typeName() const;

        std::string const op_img;
        util::sptr<Expression const> const rhs;
    };

    struct BinaryOp
        : public Expression
    {
        BinaryOp(misc::position const& pos
               , util::sptr<Expression const> l
               , std::string const& op
               , util::sptr<Expression const> r)
            : Expression(pos)
            , lhs(std::move(l))
            , op_img(op)
            , rhs(std::move(r))
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const lhs;
        std::string const op_img;
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

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

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

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

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

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> const rhs;
    };

    struct Reference
        : public Expression
    {
        Reference(misc::position const& pos, std::string const& n)
            : Expression(pos)
            , name(n)
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
    };

    struct BoolLiteral
        : public Expression
    {
        BoolLiteral(misc::position const& pos, bool v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope>
                                            , util::sref<SymbolTable>) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpz_class const&) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpf_class const&) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , bool rhs) const;

        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img) const;

        bool const value;
    public:
        static util::sptr<Expression const> mkbool(bool value);
    };

    struct IntLiteral
        : public Expression
    {
        IntLiteral(misc::position const& pos, std::string const& image)
            : Expression(pos)
            , value(image)
        {}

        IntLiteral(misc::position const& pos, mpz_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope>
                                            , util::sref<SymbolTable>) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpz_class const& rhs) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpf_class const& rhs) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , bool) const;

        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img) const;

        mpz_class const value;
    };

    struct FloatLiteral
        : public Expression
    {
        FloatLiteral(misc::position const& pos, std::string const& image)
            : Expression(pos)
            , value(image)
        {}

        FloatLiteral(misc::position const& pos, mpf_class const& v)
            : Expression(pos)
            , value(v)
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope>
                                            , util::sref<SymbolTable>) const;
        bool isLiteral() const;
        bool boolValue() const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpz_class const& rhs) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , mpf_class const& rhs) const;
        util::sptr<Expression const> operate(misc::position const& op_pos
                                           , std::string const& op_img
                                           , bool) const;

        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img
                                         , util::sptr<Expression const> lhs) const;
        util::sptr<Expression const> asRHS(misc::position const& op_pos
                                         , std::string const& op_img) const;

        mpf_class const value;
    };

    struct Call
        : public Expression
    {
        Call(misc::position const& pos
           , std::string const& n
           , std::vector<util::sptr<Expression const>> a)
                : Expression(pos)
                , name(n)
                , args(std::move(a))
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
        std::vector<util::sptr<Expression const>> const args;
    };

    struct FuncReference
        : public Expression
    {
        FuncReference(misc::position const& pos, std::string const& n, int pc)
            : Expression(pos)
            , name(n)
            , param_count(pc)
        {}

        util::sptr<proto::Expression> compile(util::sref<proto::Scope> scope
                                            , util::sref<SymbolTable> st) const;
        std::string typeName() const;
        util::sptr<Expression const> fold() const;

        std::string const name;
        int const param_count;
    };

}

#endif /* __STEKIN_FLOWCHECK_EXPRESSION_NODES_H__ */
