#include "clause-builder.h"

using namespace grammar;

void clause_builder::add_arith(int indent_len, util::sptr<expr_base const>&& arith)
{
    _stack.next_stmt(indent_len, std::move(util::mkptr(new arithmetics(arith->pos, std::move(arith)))));
}

void clause_builder::add_var_def(int indent_len, std::string const& name, util::sptr<expr_base const>&& init)
{
    _stack.next_stmt(indent_len, std::move(util::mkptr(new var_def(init->pos, name, std::move(init)))));
}

void clause_builder::add_return(int indent_len, util::sptr<expr_base const>&& ret_val)
{
    _stack.next_stmt(indent_len, std::move(util::mkptr(new func_ret(ret_val->pos, std::move(ret_val)))));
}

void clause_builder::add_void_return(int indent_len, misc::pos_type const& pos)
{
    _stack.next_stmt(indent_len, std::move(util::mkptr(new func_ret_nothing(pos))));
}

void clause_builder::add_func_def(int indent_len
                                , misc::pos_type const& pos
                                , std::string const& name
                                , std::vector<std::string> const& params)
{
    _stack.add(indent_len, std::move(util::mkmptr(new func_def_acceptor(pos, name, params))));
}

void clause_builder::add_if(int indent_len, util::sptr<expr_base const>&& condition)
{
    _stack.add(indent_len, std::move(util::mkmptr(new if_acceptor(condition->pos, std::move(condition)))));
}

void clause_builder::add_if_not(int indent_len, util::sptr<expr_base const>&& condition)
{
    _stack.add(indent_len, std::move(util::mkmptr(new ifnot_acceptor(condition->pos, std::move(condition)))));
}

void clause_builder::add_else(int indent_len, misc::pos_type const& pos)
{
    _stack.match_else(indent_len, pos);
}

void clause_builder::add_while(int indent_len, util::sptr<expr_base const>&& condition)
{
    _stack.add(indent_len, std::move(util::mkmptr(new while_acceptor(condition->pos, std::move(condition)))));
}

void clause_builder::build_and_clear()
{
    util::sptr<func_def const>(std::move(_stack.pack_all()))->compile(proto::scope::global_scope());
}
