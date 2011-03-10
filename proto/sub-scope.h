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
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& var_name);
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& func_name
                                            , std::vector<util::sptr<expr_base const>> args) const;

        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);

        util::sref<function> decl_func(misc::pos_type const& pos
                                     , std::string const& name
                                     , std::vector<std::string> const& param_names);

        util::sptr<scope> create_branch_scope();
    private:
        util::sref<symbol_table> const _symbols;
    };

}

#endif /* __STACKENING_PROTO_SUB_SCOPE_H__ */
