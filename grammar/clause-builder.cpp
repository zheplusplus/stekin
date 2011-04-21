#include "clause-builder.h"
#include "stmt-nodes.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"
#include "../report/errors.h"

using namespace grammar;

namespace {

    struct DummyAcceptor
        : public Acceptor
    {
        DummyAcceptor()
            : Acceptor(misc::position(-1))
        {}

        void accept_stmt(util::sptr<Statement const>) {}
        void accept_func(util::sptr<Function const>) {}
        void deliver_to(util::sref<Acceptor>) {}
    };

}

void AcceptorStack::add(int level, util::sptr<Acceptor> acc)
{
    _prepare_level(level, acc->pos);
    _Acceptors.push_back(std::move(acc));
}

void AcceptorStack::next_stmt(int level, util::sptr<Statement const> stmt)
{
    _prepare_level(level, stmt->pos);
    _Acceptors.back()->accept_stmt(std::move(stmt));
}

void AcceptorStack::next_func(int level, util::sptr<Function const> func)
{
    _prepare_level(level, func->pos);
    _Acceptors.back()->accept_func(std::move(func));
}

void AcceptorStack::match_else(int level, misc::position const& pos)
{
    _prepare_level(level + 1, pos);
    _Acceptors.back()->accept_else(pos);
}

void AcceptorStack::_fill_to(int level, misc::position const& pos)
{
    if (int(_Acceptors.size()) <= level) {
        error::excessive_indent(pos);
        while (int(_Acceptors.size()) <= level) {
            _Acceptors.push_back(std::move(util::mkmptr(new DummyAcceptor)));
        }
    }
}

void AcceptorStack::_shrink_to(int level)
{
    while (level + 1 < int(_Acceptors.size())) {
        util::sptr<Acceptor> deliverer(std::move(_Acceptors.back()));
        _Acceptors.pop_back();
        deliverer->deliver_to(*_Acceptors.back());
    }
}

void AcceptorStack::_prepare_level(int level, misc::position const& pos)
{
    _fill_to(level, pos);
    _shrink_to(level);
}

Block AcceptorStack::pack_all()
{
    _shrink_to(0);
    return std::move(_packer->pack());
}

AcceptorStack::AcceptorStack()
    : _packer(_prepare_first_Acceptor())
{}

util::sref<AcceptorStack::AcceptorOfPack> AcceptorStack::_prepare_first_Acceptor()
{
    util::sptr<AcceptorOfPack> packer(new AcceptorOfPack);
    util::sref<AcceptorOfPack> ref = *packer;
    _Acceptors.push_back(std::move(packer));
    return ref;
}

void AcceptorStack::AcceptorOfPack::accept_stmt(util::sptr<Statement const> stmt)
{
    _pack.add_stmt(std::move(stmt));
}

void AcceptorStack::AcceptorOfPack::accept_func(util::sptr<Function const> func)
{
    _pack.add_func(std::move(func));
}

Block AcceptorStack::AcceptorOfPack::pack()
{
    return std::move(_pack);
}

void ClauseBuilder::addArith(int indent_len, util::sptr<Expression const> arith)
{
    misc::position pos(arith->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new arithmetics(pos, std::move(arith)))));
}

void ClauseBuilder::addVarDef(int indent_len, std::string const& name, util::sptr<Expression const> init)
{
    misc::position pos(init->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new var_def(pos, name, std::move(init)))));
}

void ClauseBuilder::addReturn(int indent_len, util::sptr<Expression const> ret_val)
{
    misc::position pos(ret_val->pos);
    _stack.next_stmt(indent_len, std::move(util::mkptr(new func_ret(pos, std::move(ret_val)))));
}

void ClauseBuilder::addReturnNothing(int indent_len, misc::position const& pos)
{
    _stack.next_stmt(indent_len, std::move(util::mkptr(new func_ret_nothing(pos))));
}

void ClauseBuilder::addFunction(int indent_len
                                , misc::position const& pos
                                , std::string const& name
                                , std::vector<std::string> const& params)
{
    _stack.add(indent_len, std::move(util::mkmptr(new FunctionAcceptor(pos, name, params))));
}

void ClauseBuilder::addIf(int indent_len, util::sptr<Expression const> condition)
{
    misc::position pos(condition->pos);
    _stack.add(indent_len, std::move(util::mkmptr(new IfAcceptor(pos, std::move(condition)))));
}

void ClauseBuilder::addIfnot(int indent_len, util::sptr<Expression const> condition)
{
    misc::position pos(condition->pos);
    _stack.add(indent_len, std::move(util::mkmptr(new IfnotAcceptor(pos, std::move(condition)))));
}

void ClauseBuilder::addElse(int indent_len, misc::position const& pos)
{
    _stack.match_else(indent_len, pos);
}

flchk::Block ClauseBuilder::build_and_clear()
{
    return std::move(_stack.pack_all().compile(std::move(util::mkmptr(new flchk::filter)))->deliver());
}
