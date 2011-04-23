#ifndef __STEKIN_PROTO_SCOPE_H__
#define __STEKIN_PROTO_SCOPE_H__

#include <string>
#include <vector>
#include <list>
#include <gmpxx.h>

#include "block.h"
#include "fwd-decl.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Scope {
        virtual ~Scope() {}
    public:
        util::sptr<Expression const> makeBool(misc::position const& pos, bool value) const;
        util::sptr<Expression const> makeInt(misc::position const& pos
                                           , mpz_class const& value) const;
        util::sptr<Expression const> makeFloat(misc::position const& pos
                                             , mpf_class const& value) const;
        virtual util::sptr<Expression const> makeRef(misc::position const& pos
                                                   , std::string const& name) = 0;
        virtual util::sptr<Expression const> makeCall(
                                          misc::position const& pos
                                        , std::string const& name
                                        , std::vector<util::sptr<Expression const>> args) const = 0;
        virtual util::sptr<Expression const> makeFuncReference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count) const = 0;
        util::sptr<Expression const> makeBinary(misc::position const& pos
                                              , util::sptr<Expression const> lhs
                                              , std::string const& op
                                              , util::sptr<Expression const> rhs) const;
        util::sptr<Expression const> makePreUnary(misc::position const& pos
                                                , std::string const& op
                                                , util::sptr<Expression const> rhs) const;
        util::sptr<Expression const> makeConj(misc::position const& pos
                                            , util::sptr<Expression const> lhs
                                            , util::sptr<Expression const> rhs) const;
        util::sptr<Expression const> makeDisj(misc::position const& pos
                                            , util::sptr<Expression const> lhs
                                            , util::sptr<Expression const> rhs) const;
        util::sptr<Expression const> makeNega(misc::position const& pos
                                            , util::sptr<Expression const> rhs) const;
    public:
        virtual void defVar(misc::position const& pos, std::string const& name) = 0;
        virtual util::sref<Function> declare(misc::position const& pos
                                           , std::string const& name
                                           , std::vector<std::string> const& param_names
                                           , bool contains_void_return) = 0;
        virtual util::sptr<Scope> createBranchScope() = 0;
    public:
        void addStmt(util::sptr<Statement const> stmt);
    public:
        Block deliver();
        std::list<util::sptr<Statement const>> const& getStmts() const;
    protected:
        Block _block;
    protected:
        Scope() {}

        Scope(Scope&& rhs)
            : _block(std::move(rhs._block))
        {}

        Scope(Scope const&) = delete;
    };

}

#endif /* __STEKIN_PROTO_SCOPE_H__ */
