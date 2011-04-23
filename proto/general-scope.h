#ifndef __STEKIN_PROTO_GENERAL_SCOPE_H__
#define __STEKIN_PROTO_GENERAL_SCOPE_H__

#include <string>
#include <vector>
#include <list>

#include "scope.h"
#include "symbol-table.h"

namespace proto {

    struct GeneralScope
        : public Scope
    {
        explicit GeneralScope(util::sref<SymbolTable const> ext_symbols)
            : _symbols(ext_symbols)
        {}

        GeneralScope(GeneralScope&& rhs)
            : Scope(std::move(rhs))
            , _symbols(std::move(rhs._symbols))
        {}
    public:
        util::sptr<Expression const> makeRef(misc::position const& pos, std::string const& name);
        util::sptr<Expression const> makeCall(misc::position const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<Expression const>> args) const;
        util::sptr<Expression const> makeFuncReference(misc::position const& pos
                                                     , std::string const& name
                                                     , int param_count) const;

        void defVar(misc::position const& pos, std::string const& name);

        util::sref<Function> declare(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , bool hint_void_return);

        util::sptr<Scope> createBranchScope();
    protected:
        SymbolTable _symbols;
    protected:
        GeneralScope() {}
    };

}

#endif /* __STEKIN_PROTO_GENERAL_SCOPE_H__ */
