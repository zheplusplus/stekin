#ifndef __STACKENING_PROTO_SCOPE_H__
#define __STACKENING_PROTO_SCOPE_H__

#include <string>
#include <vector>
#include <list>

#include "node-base.h"
#include "block.h"
#include "../misc/pos-type.h"

namespace proto {

    struct symbol_table;
    struct function;

    enum termination_status {
        NO_EXPLICIT_TERMINATION,
        RETURN_VOID,
        RETURN_NO_VOID,
        PARTIAL_RETURN_VOID,
        PARTIAL_RETURN_NO_VOID,
    };

    struct scope {
        explicit scope(util::sref<symbol_table> s)
            : _symbols(s)
            , _status(NO_EXPLICIT_TERMINATION)
        {}

        virtual ~scope() {}
    public:
        util::sptr<expr_base const> make_bool(misc::pos_type const& pos, bool value) const;
        util::sptr<expr_base const> make_int(misc::pos_type const& pos, std::string const& value) const;
        util::sptr<expr_base const> make_float(misc::pos_type const& pos, std::string const& value) const;
        util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& var_name) const;
        util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                            , std::string const& func_name
                                            , std::vector<util::sptr<expr_base const>> args) const;
        util::sptr<expr_base const> make_binary(misc::pos_type const& pos
                                              , util::sptr<expr_base const> lhs
                                              , std::string const& op
                                              , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_pre_unary(misc::pos_type const& pos
                                                 , std::string const& op
                                                 , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_conj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_disj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const;
    public:
        virtual void add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val);
        virtual void add_func_ret_nothing(misc::pos_type const& pos);
        virtual void add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr);

        virtual void add_branch(misc::pos_type const& pos
                              , util::sptr<expr_base const> predicate
                              , util::sptr<scope> consequence
                              , util::sptr<scope> alternative);

        virtual void def_var(misc::pos_type const& pos
                           , std::string const& name
                           , util::sptr<expr_base const> init);
    public:
        util::sptr<scope> create_branch_scope();

        void add_custom_statement(util::sptr<stmt_base const> stmt);
    public:
        util::sref<function> decl_func(misc::pos_type const& pos
                                     , std::string const& name
                                     , std::vector<std::string> const& param_names);
    public:
        std::list<util::sptr<stmt_base const>> const& get_stmts() const;
        termination_status termination() const;
    public:
        static util::sptr<scope> global_scope();
    protected:
        block _block;
        util::sref<symbol_table> const _symbols;
        termination_status _status;
    protected:
        void _status_changed_by_sub_scope_status(termination_status sub_status);
        void _status_changed_by_return(termination_status status);
    };

    struct sub_scope
        : public scope
    {
        explicit sub_scope(util::sref<symbol_table> symbols)
            : scope(symbols)
        {}
    public:
        void def_var(misc::pos_type const& pos
                   , std::string const& name
                   , util::sptr<expr_base const> init);
    };

}

#endif /* __STACKENING_PROTO_SCOPE_H__ */
