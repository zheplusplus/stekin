#include "scope.h"
#include "function.h"
#include "node-base.h"

using namespace proto;

util::sref<Function> Scope::declare(misc::position const& pos
                                  , std::string const& name
                                  , std::vector<std::string> const& param_names
                                  , bool contains_void_return)
{
    util::sptr<Function> func(new Function(pos, name, param_names, contains_void_return));
    util::sref<Function> result = *func;
    _block.addFunc(std::move(func));
    return result;
}

void Scope::addStmt(util::sptr<Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}

std::list<util::sptr<Statement const>> const& Scope::getStmts() const
{
    return _block.getStmts();
}

Block Scope::deliver()
{
    return std::move(_block);
}
