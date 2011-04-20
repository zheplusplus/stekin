#ifndef __STEKIN_PROTO_GENERAL_SCOPE_H__
#define __STEKIN_PROTO_GENERAL_SCOPE_H__

#include <string>
#include <vector>
#include <list>

#include "scope.h"
#include "symbol-table.h"

namespace proto {

    struct general_scope
        : public scope
    {
        explicit general_scope(util::sref<symbol_table const> ext_symbols)
            : _symbols(ext_symbols)
        {}

        general_scope(general_scope&& rhs)
            : scope(std::move(rhs))
            , _symbols(std::move(rhs._symbols))
        {}
    public:
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& name);
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<expr_base const>> args) const;
        util::sptr<expr_base const> make_func_reference(misc::pos_type const& pos
                                                      , std::string const& name
                                                      , int param_count) const;

        void def_var(misc::pos_type const& pos, std::string const& name);

        util::sref<function> declare(misc::pos_type const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , bool hint_void_return);

        util::sptr<scope> create_branch_scope();
    protected:
        symbol_table _symbols;
    protected:
        general_scope() {}
    };

}

#endif /* __STEKIN_PROTO_GENERAL_SCOPE_H__ */
