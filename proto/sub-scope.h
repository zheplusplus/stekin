#ifndef __STACKENING_PROTO_SUB_SCOPE_H__
#define __STACKENING_PROTO_SUB_SCOPE_H__

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
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& name);
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<expr_base const>> args) const;
        util::sptr<expr_base const> make_func_reference(misc::pos_type const& pos
                                                      , std::string const& name
                                                      , int param_count) const;

        void def_var(misc::pos_type const&, std::string const&);

        util::sref<function> declare(misc::pos_type const&
                                   , std::string const&
                                   , std::vector<std::string> const&
                                   , bool);

        util::sptr<scope> create_branch_scope();
    private:
        util::sref<symbol_table> const _symbols;
    };

}

#endif /* __STACKENING_PROTO_SUB_SCOPE_H__ */
