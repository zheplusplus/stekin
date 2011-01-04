#ifndef __STACKENING_PROTO_SCOPE_H__
#define __STACKENING_PROTO_SCOPE_H__

#include <string>
#include <vector>

#include "../instance/node-base.h"
#include "../misc/pos-type.h"

namespace inst {
    struct space;
    struct instance_scope;
}

namespace proto {

    struct symbol_table;

    struct expr_prototype {
        virtual ~expr_prototype() {}

        virtual inst::expr_base const* inst(inst::instance_scope const* scope) const = 0;
    protected:
        expr_prototype() {}
    };

    struct stmt_prototype {
        virtual ~stmt_prototype() {}

        virtual inst::stmt_base const* inst(inst::instance_scope const* scope) const = 0;
    protected:
        stmt_prototype() {}
    };

    struct prototype_factory;
    struct func_prototype;
    struct block_prototype;

    struct scope {
        scope(block_prototype* b, symbol_table* s)
            : _block(b)
            , _symbols(s)
        {}
    public:
        expr_prototype const* make_int(misc::pos_type const& pos, std::string const& value) const;
        expr_prototype const* make_float(misc::pos_type const& pos, std::string const& value) const;
        expr_prototype const* reference(misc::pos_type const& pos, std::string const& var_name) const;
        expr_prototype const* call(misc::pos_type const& pos
                                 , std::string const& func_name
                                 , std::vector<expr_prototype const*> const& args) const;
        expr_prototype const* make_binary(misc::pos_type const& pos
                                        , expr_prototype const* lhs
                                        , std::string const& op
                                        , expr_prototype const* rhs) const;
        expr_prototype const* make_pre_unary(misc::pos_type const& pos
                                           , std::string const& op
                                           , expr_prototype const* rhs) const;
        expr_prototype const* make_conj(misc::pos_type const& pos
                                      , expr_prototype const* lhs
                                      , expr_prototype const* rhs) const;
        expr_prototype const* make_disj(misc::pos_type const& pos
                                      , expr_prototype const* lhs
                                      , expr_prototype const* rhs) const;
        expr_prototype const* make_nega(misc::pos_type const& pos, expr_prototype const* rhs) const;
    public:
        void func_ret(misc::pos_type const& pos, expr_prototype const* ret_val) const;
        void func_ret_nothing(misc::pos_type const& pos) const;
        void make_arith(misc::pos_type const& pos, expr_prototype const* expr) const;

        void branch(misc::pos_type const& pos
                  , expr_prototype const* condition
                  , stmt_prototype const* valid
                  , stmt_prototype const* invalid) const;
        void loop(misc::pos_type const& pos
                , expr_prototype const* condition
                , stmt_prototype const* body) const;

        void def_var(misc::pos_type const& pos
                   , std::string const& var_name
                   , expr_prototype const* var_init) const;
    public:
        scope const* create_branch_scope() const;
        scope const* create_loop_scope() const;
    public:
        func_prototype* def_func(misc::pos_type const& pos
                               , std::string const& func_name
                               , std::vector<std::string> const& param_names) const;
    public:
        stmt_prototype const* as_stmt() const;
    public:
        symbol_table* get_space() const;
    public:
        static scope const* global_scope();
    protected:
        block_prototype* const _block;
        symbol_table* const _symbols;
    };

}

#endif /* __STACKENING_PROTO_SCOPE_H__ */
