#include <list>
#include <set>
#include <algorithm>

#include "node-base.h"
#include "function.h"
#include "../report/errors.h"
#include "../output/func-writer.h"
#include "../util/map-compare.h"
#include "../util/pointer.h"

using namespace inst;

util::sref<Type const> Function::getReturnType() const
{
    return Type::BIT_VOID;
}

void Function::setReturnType(misc::position const& pos, util::sref<Type const> return_type)
{
    if (Type::BIT_VOID != return_type) {
        error::conflictReturnType(pos, Type::BIT_VOID->name(), return_type->name());
    }
}

bool Function::isReturnTypeResolved() const
{
    return true;
}

namespace {

    struct FunctionUnresolved
        : public Function
    {
        explicit FunctionUnresolved(util::sref<SymbolTable> st)
            : Function(st)
            , _return_type(Type::BAD_TYPE)
        {}

        util::sref<Type const> getReturnType() const
        {
            return _return_type;
        }

        void setReturnType(misc::position const& pos, util::sref<Type const> return_type)
        {
            if (Type::BAD_TYPE == _return_type) {
                _return_type = return_type;
                return;
            }
            if (_return_type != return_type) {
                error::conflictReturnType(pos, _return_type->name(), return_type->name());
            }
        }

        bool isReturnTypeResolved() const
        {
            return Type::BAD_TYPE != _return_type;
        }

        util::sref<Type const> _return_type;
    };

    struct FuncInstRecs
        : protected std::list<util::sptr<Function const>>
    {
        typedef std::list<util::sptr<Function const>> BaseType;
        typedef BaseType::const_iterator Iterator;

        void add(util::sptr<Function const> func)
        {
            push_back(std::move(func));
        }

        Iterator begin() const
        {
            return BaseType::begin();
        }

        Iterator end() const
        {
            return BaseType::end();
        }
    private:
        FuncInstRecs() {}
    public:
        static FuncInstRecs instance;
    };

    FuncInstRecs FuncInstRecs::instance;

}

util::sref<Function> Function::createInstance(util::sref<SymbolTable> st, bool has_void_returns)
{
    util::sptr<Function> func(has_void_returns ? new Function(st) : new FunctionUnresolved(st));
    util::sref<Function> fref = *func;
    FuncInstRecs::instance.add(std::move(func));
    return fref;
}

void Function::addPath(util::sref<MediateBase> path)
{
    _candidate_paths.push_back(path);
}

void Function::instNextPath()
{
    if (!hasMorePath()) {
        return;
    }
    util::sref<MediateBase> next_path = _candidate_paths.front();
    _candidate_paths.pop_front();
    next_path->mediateInst(util::mkref(*this), _symbols_or_nul_if_inst_done);
}

bool Function::hasMorePath() const
{
    return !_candidate_paths.empty();
}

void Function::_setSymbolInfo()
{
    _level = _symbols_or_nul_if_inst_done->level;
    _stack_size = _symbols_or_nul_if_inst_done->stackSize();
    _args = _symbols_or_nul_if_inst_done->getArgs();
    _symbols_or_nul_if_inst_done = util::sref<SymbolTable>(NULL);
}

void Function::instantiate(util::sref<MediateBase> mediate)
{
    addPath(mediate);
    instNextPath();
    _addStmt(mediate->inst(util::mkref(*this), _symbols_or_nul_if_inst_done));
    _setSymbolInfo();
}

static std::list<output::StackVarRec> argsToVarRecs(std::list<inst::Variable> const& args)
{
    std::list<output::StackVarRec> recs;
    std::for_each(args.begin()
                , args.end()
                , [&](inst::Variable const& var)
                  {
                      recs.push_back(output::StackVarRec(var.type->exportedName()
                                                       , var.stack_offset
                                                       , var.level));
                  });
    return recs;
}

void Function::writeDecls()
{
    std::for_each(FuncInstRecs::instance.begin()
                , FuncInstRecs::instance.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      output::writeFuncDecl(func->getReturnType()->exportedName()
                                          , func.id()
                                          , argsToVarRecs(func->_args)
                                          , func->_level
                                          , func->_stack_size);
                  });
}

void Function::writeImpls()
{
    std::for_each(FuncInstRecs::instance.begin()
                , FuncInstRecs::instance.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      output::writeFuncImpl(func->getReturnType()->exportedName(), func.id());
                      func->_block.write();
                  });
}
