#include "filter.h"
#include "node-base.h"
#include "function.h"
#include "../proto/node-base.h"

using namespace flchk;

void Filter::addReturn(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _accumulator.addReturn(pos, std::move(ret_val->fold()));
}

void Filter::addReturnNothing(misc::position const& pos)
{
    _accumulator.addReturnNothing(pos);
}

void Filter::addArith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _accumulator.addArith(pos, std::move(expr->fold()));
}

void Filter::addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence
                     , util::sptr<Filter> alternative)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->isLiteral()) {
        _accumulator.addBlock(std::move((pred->boolValue() ? consequence : alternative)
                                                                ->_accumulator));
        return;
    }
    _accumulator.addBranch(pos
                         , std::move(pred)
                         , std::move(consequence->_accumulator)
                         , std::move(alternative->_accumulator));
}

void Filter::addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->isLiteral()) {
        if (pred->boolValue()) {
            _accumulator.addBlock(std::move(consequence->_accumulator));
        }
        return;
    }
    _accumulator.addBranch(pos, std::move(pred), std::move(consequence->_accumulator));
}

void Filter::addBranchAlterOnly(misc::position const& pos
                              , util::sptr<Expression const> predicate
                              , util::sptr<Filter> alternative)
{
    util::sptr<Expression const> pred(std::move(predicate->fold()));
    if (pred->isLiteral()) {
        if (!pred->boolValue()) {
            _accumulator.addBlock(std::move(alternative->_accumulator));
        }
        return;
    }
    _accumulator.addBranchAlterOnly(pos, std::move(pred), std::move(alternative->_accumulator));
}

void Filter::defFunc(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , util::sptr<Filter> body)
{
    _defFunc(pos, name, param_names, std::move(body));
}

void Filter::compile(util::sref<proto::Scope> scope) const
{
    _accumulator.compileBlock(scope);
}

bool Filter::bodyContainsVoidReturn() const
{
    return _accumulator.containsVoidReturn();
}
