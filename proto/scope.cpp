#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "func-templ.h"
#include "err-report.h"

using namespace proto;

util::sptr<expr_base const> scope::make_bool(misc::pos_type const& pos, bool value) const
{
    return std::move(util::mkptr(new bool_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_int(misc::pos_type const& pos, std::string const& value) const
{
    return std::move(util::mkptr(new int_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_float(misc::pos_type const& pos, std::string const& value) const
{
    return std::move(util::mkptr(new float_literal(pos, value)));
}

util::sptr<expr_base const> scope::make_ref(misc::pos_type const& pos, std::string const& name) const
{
    _symbols->ref_var(pos, name);
    return std::move(util::mkptr(new reference(pos, name)));
}

util::sptr<expr_base const> scope::make_call(misc::pos_type const& pos
                                           , std::string const& name
                                           , std::vector<util::sptr<expr_base const>> args) const
{
    return std::move(util::mkptr(new call(pos, _symbols->query_func(pos, name, args.size()), std::move(args))));
}

util::sptr<expr_base const> scope::make_binary(misc::pos_type const& pos
                                             , util::sptr<expr_base const> lhs
                                             , std::string const& op
                                             , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new binary_op(pos, std::move(lhs), op, std::move(rhs))));
}

util::sptr<expr_base const> scope::make_pre_unary(misc::pos_type const& pos
                                                , std::string const& op
                                                , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new pre_unary_op(pos, op, std::move(rhs))));
}

util::sptr<expr_base const> scope::make_conj(misc::pos_type const& pos
                                           , util::sptr<expr_base const> lhs
                                           , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new conjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<expr_base const> scope::make_disj(misc::pos_type const& pos
                                           , util::sptr<expr_base const> lhs
                                           , util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new disjunction(pos, std::move(lhs), std::move(rhs))));
}

util::sptr<expr_base const> scope::make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const
{
    return std::move(util::mkptr(new negation(pos, std::move(rhs))));
}

void scope::add_func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    _flow_mgr->add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
    _flow_mgr.reset(new teminated_flow(RETURN_NO_VOID, _flow_mgr));
}

void scope::add_func_ret_nothing(misc::pos_type const& pos)
{
    _flow_mgr->add_stmt(std::move(util::mkptr(new func_ret_nothing(pos))));
    _flow_mgr.reset(new teminated_flow(RETURN_VOID, _flow_mgr));
}

void scope::add_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    _flow_mgr->add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr)))));
}

void scope::add_branch(misc::pos_type const& pos
                     , util::sptr<expr_base const> condition
                     , util::sptr<scope> valid
                     , util::sptr<scope> invalid)
{
    util::sptr<block const> valid_block(new block(std::move(valid->_block)));
    util::sptr<block const> invalid_block(new block(std::move(invalid->_block)));
    _flow_mgr->add_stmt(std::move(util::mkptr(
                new branch(pos, std::move(condition), std::move(valid_block), std::move(invalid_block)))));
}

void scope::add_loop(misc::pos_type const& pos, util::sptr<expr_base const> condition, util::sptr<scope> body)
{
    util::sptr<block const> body_block(new block(std::move(body->_block)));
    _flow_mgr->add_stmt(std::move(util::mkptr(new loop(pos, std::move(condition), std::move(body_block)))));
}

void scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init)
{
    _symbols->def_var(pos, name);
    _flow_mgr->add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

util::sptr<scope> scope::create_branch_scope()
{
    return std::move(util::mkmptr(new sub_scope(_symbols)));
}

util::sptr<scope> scope::create_loop_scope()
{
    return std::move(util::mkmptr(new sub_scope(_symbols)));
}

util::sref<func_templ> scope::decl_func(misc::pos_type const& pos
                                      , std::string const& name
                                      , std::vector<std::string> const& param_names)
{
    return _symbols->def_func(pos, name, param_names);
}

util::sptr<inst::stmt_base const> scope::inst(util::sref<inst::scope const> sc) const
{
    return std::move(_block.inst(sc));
}

termination_status scope::termination() const
{
    return _flow_mgr->termination();
}

util::sref<symbol_table> scope::get_symbols() const
{
    return _symbols;
}

util::sptr<scope> scope::global_scope()
{
    return util::sptr<scope>(0);
}

void sub_scope::def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const>)
{
    forbid_def_var(pos, name);
}
