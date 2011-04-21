#ifndef __STEKIN_PROTO_SUB_SCOPE_H__
#define __STEKIN_PROTO_SUB_SCOPE_H__

#include "fwd-decl.h"
#include "scope.h"

namespace proto {

    struct sub_scope
        : public scope
    {
        explicit sub_scope(util::sref<symbol_table> symbols)
            : _symbols(symbols)
        {}
    public:
        util::sptr<Expression const> make_ref(misc::position const& pos, std::string const& name);
        util::sptr<Expression const> make_call(misc::position const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<Expression const>> args) const;
        util::sptr<Expression const> make_FuncReference(misc::position const& pos
                                                      , std::string const& name
                                                      , int param_count) const;

        void def_var(misc::position const&, std::string const&);

        util::sref<Function> declare(misc::position const&
                                   , std::string const&
                                   , std::vector<std::string> const&
                                   , bool);

        util::sptr<scope> create_branch_scope();
    private:
        util::sref<symbol_table> const _symbols;
    };

}

#endif /* __STEKIN_PROTO_SUB_SCOPE_H__ */
