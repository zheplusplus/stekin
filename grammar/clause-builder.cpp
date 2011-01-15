#include "clause-builder.h"

using namespace grammar;

void clause_builder::add_arith(int indent_len, util::sptr<expr_base const> arith)
{
    misc::pos_type pos(arith->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new arithmetics(pos, std::move(arith)))));
}

void clause_builder::add_var_def(int indent_len, std::string const& name, util::sptr<expr_base const> init)
{
    misc::pos_type pos(init->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

void clause_builder::add_return(int indent_len, util::sptr<expr_base const> ret_val)
{
    misc::pos_type pos(ret_val->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
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

void clause_builder::add_if(int indent_len, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    _stack.add(indent_len, std::move(util::mkmptr(new if_acceptor(pos, std::move(condition)))));
}

void clause_builder::add_ifnot(int indent_len, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    _stack.add(indent_len, std::move(util::mkmptr(new ifnot_acceptor(pos, std::move(condition)))));
}

void clause_builder::add_else(int indent_len, misc::pos_type const& pos)
{
    _stack.match_else(indent_len, pos);
}

void clause_builder::add_while(int indent_len, util::sptr<expr_base const> condition)
{
    misc::pos_type pos(condition->pos);
    _stack.add(indent_len, std::move(util::mkmptr(new while_acceptor(pos, std::move(condition)))));
}

void clause_builder::build_and_clear()
{
    util::sptr<func_def const>(std::move(_stack.pack_all()))->compile(*proto::scope::global_scope());
}
