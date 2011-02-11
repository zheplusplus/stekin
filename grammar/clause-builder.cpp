#include "clause-builder.h"
#include "../report/errors.h"

using namespace grammar;

namespace {

    struct dummy_acceptor
        : public acceptor
    {
        dummy_acceptor()
            : acceptor(misc::pos_type(-1))
        {}

        void accept_stmt(util::sptr<stmt_base const>) {}
        void accept_func(util::sptr<func_def const>) {}
        void deliver_to(util::sref<acceptor>) {}
    };

}

void acceptor_stack::add(int level, util::sptr<acceptor> acc)
{
    _prepare_level(level, acc->pos);
    _acceptors.push_back(std::move(acc));
}

void acceptor_stack::next_stmt(int level, util::sptr<stmt_base const> stmt)
{
    _prepare_level(level, stmt->pos);
    _acceptors.back()->accept_stmt(std::move(stmt));
}

void acceptor_stack::next_func(int level, util::sptr<func_def const> func)
{
    _prepare_level(level, func->pos);
    _acceptors.back()->accept_func(std::move(func));
}

void acceptor_stack::match_else(int level, misc::pos_type const& pos)
{
    _prepare_level(level + 1, pos);
    _acceptors.back()->accept_else(pos);
}

void acceptor_stack::_fill_to(int level, misc::pos_type const& pos)
{
    if (int(_acceptors.size()) <= level) {
        error::excessive_indent(pos);
        while (int(_acceptors.size()) <= level) {
            _acceptors.push_back(std::move(util::mkmptr(new dummy_acceptor)));
        }
    }
}

void acceptor_stack::_shrink_to(int level)
{
    while (level + 1 < int(_acceptors.size())) {
        util::sptr<acceptor> deliverer(std::move(_acceptors.back()));
        _acceptors.pop_back();
        deliverer->deliver_to(*_acceptors.back());
    }
}

void acceptor_stack::_prepare_level(int level, misc::pos_type const& pos)
{
    _fill_to(level, pos);
    _shrink_to(level);
}

block acceptor_stack::pack_all()
{
    _shrink_to(0);
    return std::move(_packer->pack());
}

acceptor_stack::acceptor_stack()
    : _packer(_prepare_first_acceptor())
{}

util::sref<acceptor_stack::acceptor_of_pack> acceptor_stack::_prepare_first_acceptor()
{
    util::sptr<acceptor_of_pack> packer(new acceptor_of_pack);
    util::sref<acceptor_of_pack> ref = *packer;
    _acceptors.push_back(std::move(packer));
    return ref;
}

void acceptor_stack::acceptor_of_pack::accept_stmt(util::sptr<stmt_base const> stmt)
{
    _pack.add_stmt(std::move(stmt));
}

void acceptor_stack::acceptor_of_pack::accept_func(util::sptr<func_def const> func)
{
    _pack.add_func(std::move(func));
}

block acceptor_stack::acceptor_of_pack::pack()
{
    return std::move(_pack);
}

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

void clause_builder::add_return_nothing(int indent_len, misc::pos_type const& pos)
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

util::sptr<proto::scope const> clause_builder::build_and_clear()
{
    block global(std::move(_stack.pack_all()));
    util::sptr<proto::scope> scope(std::move(proto::scope::global_scope()));
    global.compile(*scope);
    return std::move(scope);
}
