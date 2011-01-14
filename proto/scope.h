#ifndef __STACKENING_PROTO_SCOPE_H__
#define __STACKENING_PROTO_SCOPE_H__

#include <string>
#include <vector>

#include "node-base.h"
#include "../misc/pos-type.h"

namespace proto {

    struct symbol_table;
    struct func_templ;
    struct block;
    struct node_factory;

    struct scope {
        scope(node_factory* f, block* b, symbol_table* s)
            : _factory(f)
            , _block(b)
            , _symbols(s)
        {}
    public:
        expr_base const* make_bool(misc::pos_type const& pos, bool value) const;
        expr_base const* make_int(misc::pos_type const& pos, std::string const& value) const;
        expr_base const* make_float(misc::pos_type const& pos, std::string const& value) const;
        expr_base const* make_ref(misc::pos_type const& pos, std::string const& var_name) const;
        expr_base const* make_call(misc::pos_type const& pos
                                 , std::string const& func_name
                                 , std::vector<expr_base const*> const& args) const;
        expr_base const* make_binary(misc::pos_type const& pos
                                   , expr_base const* lhs
                                   , std::string const& op
                                   , expr_base const* rhs) const;
        expr_base const* make_pre_unary(misc::pos_type const& pos
                                      , std::string const& op
                                      , expr_base const* rhs) const;
        expr_base const* make_conj(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs) const;
        expr_base const* make_disj(misc::pos_type const& pos, expr_base const* lhs, expr_base const* rhs) const;
        expr_base const* make_nega(misc::pos_type const& pos, expr_base const* rhs) const;
    public:
        void func_ret(misc::pos_type const& pos, expr_base const* ret_val) const;
        void func_ret_nothing(misc::pos_type const& pos) const;
        void make_arith(misc::pos_type const& pos, expr_base const* expr) const;

        void branch(misc::pos_type const& pos
                  , expr_base const* condition
                  , stmt_base const* valid
                  , stmt_base const* invalid) const;
        void loop(misc::pos_type const& pos, expr_base const* condition, stmt_base const* body) const;

        void def_var(misc::pos_type const& pos, std::string const& var_name, expr_base const* var_init) const;
    public:
        scope const* create_branch_scope() const;
        scope const* create_loop_scope() const;
    public:
        func_templ* decl_func(misc::pos_type const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names) const;
    public:
        stmt_base const* as_stmt() const;
    public:
        symbol_table* get_symbols() const;
    public:
        static scope const* global_scope();
    protected:
        node_factory* const _factory;
        block* const _block;
        symbol_table* const _symbols;
    };

}

#endif /* __STACKENING_PROTO_SCOPE_H__ */
