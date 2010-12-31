#ifndef __STACKENING_PROTO_SCOPE_H__
#define __STACKENING_PROTO_SCOPE_H__

#include <string>
#include <vector>

namespace inst {
    struct inst_space;
    struct prototype_space;
    struct type;
}

namespace proto {

    struct instance_scope;

    struct expr_instance {
        virtual ~expr_instance() {}

        virtual inst::type const* type() const = 0;
    protected:
        expr_instance() {}
    };

    struct expr_prototype {
        virtual ~expr_prototype() {}

        virtual expr_instance const* inst(instance_scope const* scope) const = 0;
    protected:
        expr_prototype() {}
    };

    struct stmt_instance {
        virtual ~stmt_instance() {}
    protected:
        stmt_instance() {}
    };

    enum termination_status {
        NO_EXPLICIT_TERMINATION,
        RETURN_VOID,
        RETURN_NO_VOID,
    };

    struct stmt_prototype {
        virtual ~stmt_prototype() {}

        virtual stmt_instance const* inst(instance_scope const* scope) const = 0;

        virtual termination_status termination() const
        {
            return NO_EXPLICIT_TERMINATION;
        }
    protected:
        stmt_prototype() {}
    };

    struct prototype_factory;
    struct func_prototype;
    struct block_prototype;

    struct prototype_scope {
        prototype_scope(prototype_factory* c, block_prototype* b, inst::prototype_space* s)
            : _factory(c)
            , _block(b)
            , _space(s)
        {}
    public:
        expr_prototype const* make_int(int lineno, std::string const& value) const;
        expr_prototype const* make_float(int lineno, std::string const& value) const;
        expr_prototype const* reference(int lineno, std::string const& var_name) const;
        expr_prototype const* call_func(int lineno
                                      , std::string const& func_name
                                      , std::vector<expr_prototype const*> const& args) const;
        expr_prototype const* make_binary(int lineno
                                        , expr_prototype const* lhs
                                        , std::string const& op
                                        , expr_prototype const* rhs) const;
        expr_prototype const*
                    make_pre_unary(int lineno, std::string const& op, expr_prototype const* rhs) const;
        expr_prototype const* make_conj(int lineno, expr_prototype const* lhs, expr_prototype const* rhs) const;
        expr_prototype const* make_disj(int lineno, expr_prototype const* lhs, expr_prototype const* rhs) const;
        expr_prototype const* make_nega(int lineno, expr_prototype const* rhs) const;
    public:
        void func_ret(int lineno, expr_prototype const* ret_val) const;
        void func_ret_nothing(int lineno) const;
        void make_arith(int lineno, expr_prototype const* expr) const;

        void branch(int lineno
                  , expr_prototype const* condition
                  , stmt_prototype const* valid
                  , stmt_prototype const* invalid) const;
        void loop(int lineno, expr_prototype const* condition, stmt_prototype const* body) const;

        void def_var(int lineno, std::string const& var_name, expr_prototype const* var_init) const;
    public:
        prototype_scope const* create_branch_scope() const;
        prototype_scope const* create_loop_scope() const;
    public:
        func_prototype* def_func(int lineno
                               , std::string const& func_name
                               , std::vector<std::string> const& param_names) const;
    public:
        stmt_prototype const* as_stmt() const;
    public:
        inst::prototype_space* get_space() const;
    public:
        static prototype_scope const* global_scope();
    protected:
        prototype_factory* const _factory;
        block_prototype* const _block;
        inst::prototype_space* const _space;
    };

}

#endif /* __STACKENING_PROTO_SCOPE_H__ */
