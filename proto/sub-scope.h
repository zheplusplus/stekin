#ifndef __STEKIN_PROTO_SUB_SCOPE_H__
#define __STEKIN_PROTO_SUB_SCOPE_H__

#include "fwd-decl.h"
#include "scope.h"

namespace proto {

    struct SubScope
        : public Scope
    {
        explicit SubScope(util::sref<SymbolTable> symbols)
            : _symbols(symbols)
        {}
    public:
        util::sptr<Expression const> makeRef(misc::position const& pos, std::string const& name);
        util::sptr<Expression const> makeCall(misc::position const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<Expression const>> args) const;
        util::sptr<Expression const> makeFuncReference(misc::position const& pos
                                                      , std::string const& name
                                                      , int param_count) const;

        void defVar(misc::position const&, std::string const&);

        util::sref<Function> declare(misc::position const&
                                   , std::string const&
                                   , std::vector<std::string> const&
                                   , bool);

        util::sptr<Scope> createBranchScope();
    private:
        util::sref<SymbolTable> const _symbols;
    };

}

#endif /* __STEKIN_PROTO_SUB_SCOPE_H__ */
