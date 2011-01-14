#ifndef __STACKENING_PROTO_NODE_FACTORY_H__
#define __STACKENING_PROTO_NODE_FACTORY_H__

#include <list>

#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "scope.h"

namespace proto {

    struct node_factory {
        virtual int_literal const* make_int(misc::pos_type const& pos, std::string const& value);
        virtual float_literal const* make_float(misc::pos_type const& pos, std::string const& value);
        virtual reference const* make_ref(misc::pos_type const& pos, std::string const& name);
        virtual call const* call_func(misc::pos_type const& pos
                                    , func_templ* func
                                    , std::vector<expr_base const*> const& args);
        virtual binary_op const* make_binary(misc::pos_type const& pos
                                                  , expr_base const* lhs
                                                  , std::string const& op
                                                  , expr_base const* rhs);
        virtual pre_unary_op const*
                    make_pre_unary(misc::pos_type const& pos, std::string const& op, expr_base const* rhs);
        virtual conjunction const* make_conj(misc::pos_type const& pos
                                           , expr_base const* lhs
                                           , expr_base const* rhs);
        virtual disjunction const* make_disj(misc::pos_type const& pos
                                           , expr_base const* lhs
                                           , expr_base const* rhs);
        virtual negation const* make_nega(misc::pos_type const& pos, expr_base const* rhs);
    public:
        virtual func_ret const* make_ret(misc::pos_type const& pos, expr_base const* ret_val);
        virtual func_ret_nothing const* make_ret_nothing(misc::pos_type const& pos);
        virtual arithmetics const* make_arith(misc::pos_type const& pos, expr_base const* expr);
        virtual branch const* make_branch(misc::pos_type const& pos
                                        , expr_base const* condition
                                        , stmt_base const* valid
                                        , stmt_base const* invalid);
        virtual loop const*
                    make_loop(misc::pos_type const& pos, expr_base const* condition, stmt_base const* body);
        virtual var_def const* def_var(misc::pos_type const& pos
                                     , std::string const& name
                                     , expr_base const* var_init);
    public:
        virtual scope const* create_branch_scope() = 0;
        virtual scope const* create_loop_scope() = 0;
    public:
        virtual block* create_block();
    protected:
        node_factory() {}
        node_factory(node_factory&& rhs) {}
    protected:
        std::list<int_literal> _ints;
        std::list<float_literal> _floats;
        std::list<reference> _refs;
        std::list<call> _calls;
        std::list<binary_op> _binaries;
        std::list<pre_unary_op> _pre_unaries;
        std::list<conjunction> _conjs;
        std::list<disjunction> _disjs;
        std::list<negation> _negas;

        std::list<block> _blocks;
        std::list<arithmetics> _ariths;
        std::list<branch> _branches;
        std::list<loop> _loops;
        std::list<var_def> _var_defs;
        std::list<func_ret> _returns;
        std::list<func_ret_nothing> _void_returns;

        std::list<scope> _sub_scopes;

        node_factory(node_factory const&) = delete;
    };

}

#endif /* __STACKENING_PROTO_NODE_FACTORY_H__ */
