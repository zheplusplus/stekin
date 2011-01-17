#include "acceptor.h"
#include "err-report.h"

using namespace grammar;

void acceptor::accept_else(misc::pos_type const& else_pos)
{
    else_not_match_if(else_pos);
}

void func_def_forbid_acceptor::accept_func(util::sptr<func_def const> func)
{
    forbid_def_func(func->pos, func->name);
}

void if_acceptor::accept_stmt(util::sptr<stmt_base const> stmt)
{
    _current_branch->add_stmt(std::move(stmt));
}

void if_acceptor::accept_func(util::sptr<func_def const> func)
{
    _current_branch->add_func(std::move(func));
}

void if_acceptor::deliver_to(util::sptr<acceptor>& acc)
{
    acc->accept_stmt(std::move(util::mkptr(
                            new branch(pos, std::move(_condition), std::move(_valid), std::move(_invalid)))));
}

void if_acceptor::accept_else(misc::pos_type const& else_pos)
{
    if (_else_matched()) {
        if_already_match_else(*_last_else_pos, else_pos);
    } else {
        _current_branch = &_invalid;
        _last_else_pos.reset(new misc::pos_type(else_pos));
    }
}

bool if_acceptor::_else_matched() const
{
    return bool(_last_else_pos);
}

void ifnot_acceptor::accept_stmt(util::sptr<stmt_base const> stmt)
{
    _invalid.add_stmt(std::move(stmt));
}

void ifnot_acceptor::accept_func(util::sptr<func_def const> func)
{
    _invalid.add_func(std::move(func));
}

void ifnot_acceptor::deliver_to(util::sptr<acceptor>& acc)
{
    acc->accept_stmt(std::move(util::mkptr(
                            new branch(pos, std::move(_condition), std::move(block()), std::move(_invalid)))));
}

void while_acceptor::accept_stmt(util::sptr<stmt_base const> stmt)
{
    _body.add_stmt(std::move(stmt));
}

void while_acceptor::accept_func(util::sptr<func_def const> func)
{
    _body.add_func(std::move(func));
}

void while_acceptor::deliver_to(util::sptr<acceptor>& acc)
{
    acc->accept_stmt(std::move(util::mkptr(new loop(pos, std::move(_condition), std::move(_body)))));
}

void func_def_acceptor::accept_stmt(util::sptr<stmt_base const> stmt)
{
    _body.add_stmt(std::move(stmt));
}

void func_def_acceptor::accept_func(util::sptr<func_def const> func)
{
    _body.add_func(std::move(func));
}

void func_def_acceptor::deliver_to(util::sptr<acceptor>& acc)
{
    acc->accept_func(std::move(util::mkptr(new func_def(pos, name, param_names, std::move(_body)))));
}

namespace {

    struct dummy_acceptor
        : public acceptor
    {
        dummy_acceptor()
            : acceptor(misc::pos_type(-1))
        {}

        void accept_stmt(util::sptr<stmt_base const>) {}
        void accept_func(util::sptr<func_def const>) {}
        void deliver_to(util::sptr<acceptor>&) {}
        void accept_else(misc::pos_type const& else_pos) {}
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
    _acceptors[level]->accept_stmt(std::move(stmt));
}

void acceptor_stack::next_func(int level, util::sptr<func_def const> func)
{
    _prepare_level(level, func->pos);
    _acceptors[level]->accept_func(std::move(func));
}

void acceptor_stack::match_else(int level, misc::pos_type const& pos)
{
    _prepare_level(level, pos);
    _acceptors[level]->accept_else(pos);
}

void acceptor_stack::_fill_to(int level, misc::pos_type const& pos)
{
    if (int(_acceptors.size()) <= level) {
        excessive_indent(pos);
        while (int(_acceptors.size()) <= level) {
            _acceptors.push_back(std::move(util::mkmptr(new dummy_acceptor)));
        }
    }
}

void acceptor_stack::_shrink_to(int level)
{
    while (level < int(_acceptors.size())) {
        _acceptors[level]->deliver_to(_acceptors[level - 1]);
        _acceptors.pop_back();
    }
}

void acceptor_stack::_prepare_level(int level, misc::pos_type const& pos)
{
    _fill_to(level, pos);
    _shrink_to(level);
}

acceptor_stack::acceptor_stack()
{
    _acceptors.push_back(std::move(util::mkmptr(
                                new func_def_acceptor(misc::pos_type(-1), "", std::vector<std::string>()))));
}

util::sptr<func_def const> acceptor_stack::pack_all()
{
    _shrink_to(0);

    struct acceptor_of_pack
        : public acceptor
    {
        explicit acceptor_of_pack(util::sptr<func_def const>& f)
            : acceptor(misc::pos_type(-1))
            , func(f)
        {}

        void accept_stmt(util::sptr<stmt_base const>) {}
        void deliver_to(util::sptr<acceptor>&) {}
        void accept_else(misc::pos_type const& else_pos) {}

        void accept_func(util::sptr<func_def const> func)
        {
            func = std::move(func);
        }

        util::sptr<func_def const>& func;
    };

    util::sptr<func_def const> func;
    util::sptr<acceptor> packer(new acceptor_of_pack(func));
    _acceptors[0]->deliver_to(packer);
    return std::move(func);
}
