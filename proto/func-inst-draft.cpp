#include <list>
#include <set>
#include <algorithm>

#include "func-inst-draft.h"
#include "node-base.h"
#include "symbol-table.h"
#include "type.h"
#include "variable.h"
#include "../report/errors.h"
#include "../output/func-writer.h"
#include "../util/map-compare.h"
#include "../util/pointer.h"

using namespace proto;

namespace {

    struct FuncInstDraftUnresolved
        : public FuncInstDraft
    {
        explicit FuncInstDraftUnresolved(util::sref<SymbolTable> st)
            : FuncInstDraft(st)
            , _return_type_or_nul_if_not_set(NULL)
        {}

        util::sref<Type const> getReturnType() const
        {
            if (_return_type_or_nul_if_not_set.nul()) {
                return Type::BAD_TYPE;
            }
            return _return_type_or_nul_if_not_set;
        }

        void setReturnType(misc::position const& pos, util::sref<Type const> return_type)
        {
            if (_return_type_or_nul_if_not_set.nul()
                || Type::BAD_TYPE == _return_type_or_nul_if_not_set)
            {
                _return_type_or_nul_if_not_set = return_type;
                return;
            }
            if (_return_type_or_nul_if_not_set != return_type) {
                error::conflictReturnType(pos
                                        , _return_type_or_nul_if_not_set->name()
                                        , return_type->name());
            }
        }

        bool isReturnTypeResolved() const
        {
            return _return_type_or_nul_if_not_set.not_nul();
        }

        util::sref<Type const> _return_type_or_nul_if_not_set;
    };

    struct FuncInstRecs
        : protected std::list<util::sptr<FuncInstDraft const>>
    {
        typedef std::list<util::sptr<FuncInstDraft const>> BaseType;
        typedef BaseType::const_iterator Iterator;

        void add(util::sptr<FuncInstDraft const> func)
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

util::sref<Type const> FuncInstDraft::getReturnType() const
{
    return Type::BIT_VOID;
}

void FuncInstDraft::setReturnType(misc::position const& pos, util::sref<Type const> return_type)
{
    if (Type::BIT_VOID != return_type) {
        error::conflictReturnType(pos, Type::BIT_VOID->name(), return_type->name());
    }
}

bool FuncInstDraft::isReturnTypeResolved() const
{
    return true;
}

util::sref<FuncInstDraft> FuncInstDraft::create(util::sref<SymbolTable> st, bool has_void_returns)
{
    util::sptr<FuncInstDraft> func(has_void_returns ? new FuncInstDraft(st)
                                                    : new FuncInstDraftUnresolved(st));
    util::sref<FuncInstDraft> fref = *func;
    FuncInstRecs::instance.add(std::move(func));
    return fref;
}

util::sref<FuncInstDraft> FuncInstDraft::badDraft()
{
    static SymbolTable st;
    static FuncInstDraft bad_draft(util::mkref(st));
    return util::mkref(bad_draft);
}

void FuncInstDraft::addPath(util::sref<Statement> path)
{
    _candidate_paths.push_back(path);
}

void FuncInstDraft::instNextPath()
{
    if (!hasMorePath()) {
        return;
    }
    util::sref<Statement> next_path = _candidate_paths.front();
    _candidate_paths.pop_front();
    next_path->mediateInst(util::mkref(*this), _symbols_or_nul_if_inst_done);
}

bool FuncInstDraft::hasMorePath() const
{
    return !_candidate_paths.empty();
}

void FuncInstDraft::_setSymbolInfo()
{
    _level = _symbols_or_nul_if_inst_done->level;
    _stack_size = _symbols_or_nul_if_inst_done->stackSize();
    _args = _symbols_or_nul_if_inst_done->getArgs();
    _symbols_or_nul_if_inst_done = util::sref<SymbolTable>(NULL);
}

void FuncInstDraft::_addStmt(util::sptr<inst::Statement const> stmt)
{
    _block.addStmt(std::move(stmt));
}

void FuncInstDraft::instantiate(util::sref<Statement> stmt)
{
    addPath(stmt);
    instNextPath();
    _addStmt(stmt->inst(util::mkref(*this), _symbols_or_nul_if_inst_done));
    _setSymbolInfo();
}

static std::list<output::StackVarRec> argsToVarRecs(std::list<Variable> const& args)
{
    std::list<output::StackVarRec> recs;
    std::for_each(args.begin()
                , args.end()
                , [&](Variable const& var)
                  {
                      recs.push_back(output::StackVarRec(var.type->exportedName()
                                                       , var.stack_offset
                                                       , var.level));
                  });
    return recs;
}

void FuncInstDraft::writeDecls()
{
    std::for_each(FuncInstRecs::instance.begin()
                , FuncInstRecs::instance.end()
                , [&](util::sptr<FuncInstDraft const> const& func)
                  {
                      output::writeFuncDecl(func->getReturnType()->exportedName()
                                          , func.id()
                                          , argsToVarRecs(func->_args)
                                          , func->_level
                                          , func->_stack_size);
                  });
}

void FuncInstDraft::writeImpls()
{
    std::for_each(FuncInstRecs::instance.begin()
                , FuncInstRecs::instance.end()
                , [&](util::sptr<FuncInstDraft const> const& func)
                  {
                      output::writeFuncImpl(func->getReturnType()->exportedName(), func.id());
                      func->_block.write();
                  });
}
