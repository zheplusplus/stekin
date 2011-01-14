#include <algorithm>
#include <cassert>

#include "../../proto/func-templ.h"
#include "../../proto/symbol-table.h"

using namespace test;
using namespace proto;

namespace {

    struct func_adaptor
        : public stmt_base
    {
        func_adaptor(func_prototype* f)
            : func(f)
        {}

        stmt_instance const* inst(instance_scope const*) const
        {
            func->inst(0, NULL, std::vector<symtab::type const*>());
            return NULL;
        }

        func_prototype* const func;
    };

    struct phony_call
        : public call_prototype
    {
        phony_call(int lineno, std::string const& n, std::vector<expr_base const*> const& a)
            : call_prototype(lineno, NULL, a)
            , name(n)
        {}

        call_instance const* inst(instance_scope const*) const;

        std::string const name;
    };

    struct phony_int_literal
        : public int_literal_prototype
    {
        phony_int_literal(int lineno, std::string const& i)
            : int_literal_prototype(lineno, i)
            , image(i)
        {}

        int_literal_instance const* inst(instance_scope const*) const;

        std::string const image;
    };

    struct phony_float_literal
        : public float_literal_prototype
    {
        phony_float_literal(int lineno, std::string const& i)
            : float_literal_prototype(lineno, i)
            , image(i)
        {}

        float_literal_instance const* inst(instance_scope const*) const;

        std::string const image;
    };

}

static std::list<phony_int_literal> int_literals;
static std::list<phony_float_literal> float_literals;
static std::list<phony_call> calls;
static std::list<ref_prototype> var_refs;
static std::list<binary_prototype> binary_ops;
static std::list<pre_unary_prototype> pre_unary_ops;
static std::list<conj_prototype> conjs;
static std::list<disj_prototype> disjs;
static std::list<nega_prototype> negas;

static std::list<block_prototype> blocks;
static std::list<arith_prototype> ariths;
static std::list<branch_prototype> branches;
static std::list<loop_prototype> loops;
static std::list<var_def_prototype> var_defs;
static std::list<return_prototype> returnings;
static std::list<void_return_prototype> void_returnings;

static std::list<prototype_scope> scopes;
static std::list<func_adaptor> funcs;
static std::list<func_prototype> func_defs;

static prototype_scope* create_scope()
{
    blocks.push_back(block_prototype());
    scopes.push_back(prototype_scope(NULL, &blocks.back(), NULL));
    return &scopes.back();
}

expr_base const* prototype_scope::make_int(int lineno, std::string const& value) const
{
    int_literals.push_back(phony_int_literal(lineno, value));
    return &int_literals.back();
}

expr_base const* prototype_scope::make_float(int lineno, std::string const& value) const
{
    float_literals.push_back(phony_float_literal(lineno, value));
    return &float_literals.back();
}

expr_base const* prototype_scope::reference(int lineno, std::string const& var_name) const
{
    var_refs.push_back(ref_prototype(lineno, var_name));
    return &var_refs.back();
}

expr_base const* prototype_scope::call_func(int lineno
                                               , std::string const& name
                                               , std::vector<expr_base const*> const& args) const
{
    calls.push_back(phony_call(lineno, name, args));
    return &calls.back();
}

expr_base const* prototype_scope::make_binary(int lineno
                                                 , expr_base const* lhs
                                                 , std::string const& op
                                                 , expr_base const* rhs) const
{
    binary_ops.push_back(binary_prototype(lineno, lhs, op, rhs));
    return &binary_ops.back();
}

expr_base const*
    prototype_scope::make_pre_unary(int lineno, std::string const& op, expr_base const* rhs) const
{
    pre_unary_ops.push_back(pre_unary_prototype(lineno, op, rhs));
    return &pre_unary_ops.back();
}

expr_base const* prototype_scope::make_conj(int lineno
                                               , expr_base const* lhs
                                               , expr_base const* rhs) const
{
    conjs.push_back(conj_prototype(lineno, lhs, rhs));
    return &conjs.back();
}

expr_base const* prototype_scope::make_disj(int lineno
                                               , expr_base const* lhs
                                               , expr_base const* rhs) const
{
    disjs.push_back(disj_prototype(lineno, lhs, rhs));
    return &disjs.back();
}

expr_base const* prototype_scope::make_nega(int lineno, expr_base const* rhs) const
{
    negas.push_back(nega_prototype(lineno, rhs));
    return &negas.back();
}


void prototype_scope::func_ret(int lineno, expr_base const* ret_val) const
{
    returnings.push_back(return_prototype(lineno, ret_val));
    _block->add_stmt(&returnings.back());
}

void prototype_scope::func_ret_nothing(int lineno) const
{
    void_returnings.push_back(void_return_prototype(lineno));
    _block->add_stmt(&void_returnings.back());
}

void prototype_scope::make_arith(int lineno, expr_base const* expr) const
{
    ariths.push_back(arith_prototype(lineno, expr));
    _block->add_stmt(&ariths.back());
}

termination_status branch_prototype::termination() const
{
    return NO_EXPLICIT_TERMINATION;
}

void prototype_scope::branch(int lineno
                           , expr_base const* condition
                           , stmt_base const* valid
                           , stmt_base const* invalid) const
{
    branches.push_back(branch_prototype(lineno, condition, valid, invalid));
    _block->add_stmt(&branches.back());
}

void prototype_scope::loop(int lineno, expr_base const* condition, stmt_base const* body) const
{
    loops.push_back(loop_prototype(lineno, condition, body));
    _block->add_stmt(&loops.back());
}

void prototype_scope::def_var(int lineno, std::string const& var_name, expr_base const* var_init) const
{
    var_defs.push_back(var_def_prototype(lineno, var_name, var_init));
    _block->add_stmt(&var_defs.back());
}

func_prototype* prototype_scope::def_func(int lineno
                                        , std::string const& func_name
                                        , std::vector<std::string> const& param_names) const
{
    func_defs.push_back(std::move(func_prototype(lineno, func_name, param_names)));
    funcs.push_back(func_adaptor(&func_defs.back()));
    _block->add_stmt(&funcs.back());
    return funcs.back().func;
}

prototype_scope const* prototype_scope::create_branch_scope() const
{
    return create_scope();
}

prototype_scope const* prototype_scope::create_loop_scope() const
{
    return create_scope();
}

stmt_base const* prototype_scope::as_stmt() const
{
    return _block;
}

prototype_scope const* prototype_scope::global_scope()
{
    static block_prototype block;
    static prototype_scope ins(NULL, &block, NULL);
    return &ins;
}

int_literal_instance const* int_literal_prototype::inst(instance_scope const*) const
{
    assert(!"this func shall be shadowed.");
    return NULL;
}

float_literal_instance const* float_literal_prototype::inst(instance_scope const*) const
{
    assert(!"this func shall be shadowed.");
    return NULL;
}

call_instance const* call_prototype::inst(instance_scope const*) const
{
    assert(!"this func shall be shadowed.");
    return NULL;
}

int_literal_instance const* phony_int_literal::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, INT_LITERAL, image);
    return NULL;
}

float_literal_instance const* phony_float_literal::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, FLOAT_LITERAL, image);
    return NULL;
}

call_instance const* phony_call::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, FUNC_CALL_BEGIN, name);
    std::for_each(args.begin()
                , args.end()
                , [&](expr_base const* expr)
                  {
                      node_verifier::syntax_gen()(line_num, ARGUMENT);
                      expr->inst(NULL);
                  });
    node_verifier::syntax_gen()(line_num, FUNC_CALL_END);
    return NULL;
}

pre_unary_instance const* pre_unary_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, PRE_UNARY_OP_BEGIN, op)(line_num, OPERAND);
    rhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, PRE_UNARY_OP_END);
    return NULL;
}

binary_instance const* binary_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, BINARY_OP_BEGIN, op)(line_num, OPERAND);
    lhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, OPERAND);
    rhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, BINARY_OP_END);
    return NULL;
}

conj_instance const* conj_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, BINARY_OP_BEGIN, "&&")(line_num, OPERAND);
    lhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, OPERAND);
    rhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, BINARY_OP_END);
    return NULL;
}

disj_instance const* disj_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, BINARY_OP_BEGIN, "||")(line_num, OPERAND);
    lhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, OPERAND);
    rhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, BINARY_OP_END);
    return NULL;
}

nega_instance const* nega_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, PRE_UNARY_OP_BEGIN, "!")(line_num, OPERAND);
    rhs->inst(NULL);
    node_verifier::syntax_gen()(line_num, PRE_UNARY_OP_END);
    return NULL;
}

ref_instance const* ref_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, VARIABLE, name);
    return NULL;
}

void block_prototype::add_stmt(stmt_base const* stmt)
{
    _stmts.push_back(stmt);
}

block_instance const* block_prototype::inst(instance_scope const*) const 
{
    node_verifier::syntax_gen()(0, BLOCK_BEGIN);
    std::for_each(_stmts.begin()
                , _stmts.end()
                , [&](stmt_base const* stmt)
                  {
                      stmt->inst(NULL);
                  });
    node_verifier::syntax_gen()(0, BLOCK_END);
    return NULL;
}

arith_instance const* arith_prototype::inst(instance_scope const*) const
{
    expr->inst(NULL);
    return NULL;
}

branch_instance const* branch_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, BRANCH_BEGIN);
    condition->inst(NULL);
    valid->inst(NULL);
    invalid->inst(NULL);
    node_verifier::syntax_gen()(line_num, BRANCH_END);
    return NULL;
}

loop_instance const* loop_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, LOOP_BEGIN);
    condition->inst(NULL);
    body->inst(NULL);
    node_verifier::syntax_gen()(line_num, LOOP_END);
    return NULL;
}

return_instance const* return_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, RETURN);
    ret_val->inst(NULL);
    return NULL;
}

void_return_instance const* void_return_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, RETURN, "");
    return NULL;
}

obj_ctor_instance const* var_def_prototype::inst(instance_scope const*) const
{
    node_verifier::syntax_gen()(line_num, VAR_DEF, name);
    init->inst(NULL);
    return NULL;
}

func_prototype::func_prototype(int lineno, std::string const& func_name, std::vector<std::string> const& ps)
    : line_num(lineno)
    , name(func_name)
    , params(ps)
    , _factory(&_space)
    , scope(*create_scope())
{}

func_prototype::func_prototype(func_prototype&& rhs)
    : line_num(rhs.line_num)
    , name(rhs.name)
    , params(rhs.params)
    , _factory(&_space)
    , scope(rhs.scope)
{}
