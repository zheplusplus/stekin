#include "accumulator.h"
#include "stmt-nodes.h"
#include "function.h"
#include "filter.h"
#include "../proto/node-base.h"
#include "../report/errors.h"
#include "../report/warnings.h"

using namespace flchk;

void Accumulator::addReturn(misc::position const& pos, util::sptr<Expression const> ret_val)
{
    _checkNotTerminated(pos);
    _setTerminatedNotByVoidReturn(pos);
    _block.addStmt(util::mkptr(new Return(pos, std::move(ret_val))));
}

void Accumulator::addReturnNothing(misc::position const& pos)
{
    _checkNotTerminated(pos);
    _setTerminatedByVoidReturn(pos);
    _block.addStmt(util::mkptr(new ReturnNothing(pos)));
}

void Accumulator::addArith(misc::position const& pos, util::sptr<Expression const> expr)
{
    _checkNotTerminated(pos);
    _block.addStmt(util::mkptr(new Arithmetics(pos, std::move(expr))));
}

void Accumulator::addBranch(misc::position const& pos
                          , util::sptr<Expression const> predicate
                          , Accumulator consequence
                          , Accumulator alternative)
{
    _checkNotTerminated(pos);
    _checkBranchesTermination(consequence, alternative);
    _setTerminationBySubAccumulator(consequence);
    _setTerminationBySubAccumulator(alternative);
    _block.addStmt(util::mkptr(new Branch(pos
                                        , std::move(predicate)
                                        , std::move(consequence._block)
                                        , std::move(alternative._block))));
}

void Accumulator::addBranch(misc::position const& pos
                          , util::sptr<Expression const> predicate
                          , Accumulator consequence)
{
    _checkNotTerminated(pos);
    _setTerminationBySubAccumulator(consequence);
    _block.addStmt(util::mkptr(new Branch(pos
                                        , std::move(predicate)
                                        , std::move(consequence._block)
                                        , std::move(Block()))));
}

void Accumulator::addBranchAlterOnly(misc::position const& pos
                                   , util::sptr<Expression const> predicate
                                   , Accumulator alternative)
{
    _checkNotTerminated(pos);
    _setTerminationBySubAccumulator(alternative);
    _block.addStmt(util::mkptr(new Branch(pos
                                        , std::move(predicate)
                                        , std::move(Block())
                                        , std::move(alternative._block))));
}

void Accumulator::addBlock(Accumulator b)
{
    _block.append(std::move(b._block));
    _setSelfTerminated(std::move(b));
}

void Accumulator::defVar(misc::position const& pos
                       , std::string const& name
                       , util::sptr<Expression const> init)
{
    _checkNotTerminated(pos);
    _block.addStmt(util::mkptr(new VarDef(pos, name, std::move(init))));
}

util::sref<Function> Accumulator::defFunc(misc::position const& pos
                                        , std::string const& name
                                        , std::vector<std::string> const& param_names
                                        , util::sptr<Filter> body)
{
    return _block.defFunc(pos, name, param_names, std::move(body));
}

void Accumulator::compileBlock(util::sref<proto::Scope> scope, util::sref<SymbolTable> st) const
{
    _block.compile(scope, st);
}

bool Accumulator::containsVoidReturn() const
{
    return _contains_void_return || !_terminated();
}

void Accumulator::_setTerminatedByVoidReturn(misc::position const& pos)
{
    _setTerminatedNotByVoidReturn(pos);
    _contains_void_return = true;
}

void Accumulator::_setTerminatedNotByVoidReturn(misc::position const& pos)
{
    _termination_pos.reset(new misc::position(pos));
}

void Accumulator::_setTerminationBySubAccumulator(Accumulator const& sub)
{
    _contains_void_return = _contains_void_return || sub._contains_void_return;
}

void Accumulator::_checkBranchesTermination(Accumulator const& consequence
                                          , Accumulator const& alternative)
{
    if (consequence._terminated() || alternative._terminated()) {
        warning::oneOrTwoBranchesTerminated(*consequence._termination_pos
                                          , *alternative._termination_pos);
    }
}

void Accumulator::_checkNotTerminated(misc::position const& pos)
{
    if (_terminated()) {
        _reportTerminated(pos);
    }
}

void Accumulator::_reportTerminated(misc::position const& pos)
{
    if (!_error_reported) {
        error::flowTerminated(pos, _termination_pos.cp());
        _error_reported = true;
    }
}

bool Accumulator::_terminated() const
{
    return bool(_termination_pos);
}

void Accumulator::_setSelfTerminated(Accumulator term)
{
    _setTerminationBySubAccumulator(term);
    _termination_pos = std::move(term._termination_pos);
    if (bool(_termination_pos)) {
        _error_reported = true;
    }
}
