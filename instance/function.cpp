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

Variable Function::defVar(misc::position const& pos, util::sref<Type const> type, std::string const& name)
{
    return _symbols.defVar(pos, type, name);
}

Variable Function::queryVar(misc::position const& pos, std::string const& name) const
{
    return _symbols.queryVar(pos, name);
}

util::sref<Type const> Function::get_return_type() const
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

    struct Function_unresolved
        : public Function
    {
        Function_unresolved(int ext_level
                          , std::list<ArgNameTypeRec> const& args
                          , std::map<std::string, Variable const> const& extvars)
            : Function(ext_level, args, extvars)
            , _return_type(Type::BAD_TYPE)
        {}

        util::sref<Type const> get_return_type() const
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

    struct func_inst_recs
        : protected std::list<util::sptr<Function const>>
    {
        typedef std::list<util::sptr<Function const>> BaseType;
        typedef BaseType::const_iterator iterator;

        void add(util::sptr<Function const> func)
        {
            push_back(std::move(func));
        }

        iterator begin() const
        {
            return BaseType::begin();
        }

        iterator end() const
        {
            return BaseType::end();
        }
    private:
        func_inst_recs() {}
    public:
        static func_inst_recs instance;
    };

    func_inst_recs func_inst_recs::instance;

}

util::sref<Function> Function::createInstance(int ext_level
                                             , std::list<ArgNameTypeRec> const& arg_types
                                             , std::map<std::string, Variable const> const& extvars
                                             , bool has_void_returns)
{
    util::sptr<Function> func(has_void_returns ? new Function(ext_level, arg_types, extvars)
                                               : new Function_unresolved(ext_level, arg_types, extvars));
    util::sref<Function> fref = *func;
    func_inst_recs::instance.add(std::move(func));
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
    next_path->mediateInst(util::mkref(*this));
}

bool Function::hasMorePath() const
{
    return !_candidate_paths.empty();
}

int Function::level() const
{
    return _symbols.level;
}

static std::list<output::stack_var_record> args_to_var_recs(std::list<inst::Variable> const& args)
{
    std::list<output::stack_var_record> recs;
    std::for_each(args.begin()
                , args.end()
                , [&](inst::Variable const& var)
                  {
                      recs.push_back(output::stack_var_record(var.type->exportedName()
                                                            , var.stack_offset
                                                            , var.level));
                  });
    return recs;
}

void Function::write_decls()
{
    std::for_each(func_inst_recs::instance.begin()
                , func_inst_recs::instance.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      output::write_func_decl(func->get_return_type()->exportedName()
                                            , func.id()
                                            , args_to_var_recs(func->_symbols.get_args())
                                            , func->_symbols.level
                                            , func->_symbols.stack_size());
                  });
}

void Function::write_impls()
{
    std::for_each(func_inst_recs::instance.begin()
                , func_inst_recs::instance.end()
                , [&](util::sptr<Function const> const& func)
                  {
                      output::write_func_perform_impl(func->get_return_type()->exportedName(), func.id());
                      func->_block.write();
                  });
}
