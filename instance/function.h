#ifndef __STEKIN_INSTANCE_FUNCTION_H__
#define __STEKIN_INSTANCE_FUNCTION_H__

#include <map>
#include <list>
#include <string>

#include "scope.h"
#include "stmt-nodes.h"
#include "type.h"
#include "inst-mediate.h"
#include "symbol-table.h"
#include "../misc/pos-type.h"

namespace inst {

    struct Function
        : public Scope
    {
        Variable defVar(misc::position const& pos, util::sref<Type const> type, std::string const& name);
        Variable queryVar(misc::position const& pos, std::string const& name) const;

        void setReturnType(misc::position const& pos, util::sref<Type const> return_type);
        virtual util::sref<Type const> get_return_type() const;
        virtual bool isReturnTypeResolved() const;

        void addPath(util::sref<MediateBase> path);
        void instNextPath();
        bool hasMorePath() const;

        int level() const;
    public:
        static util::sref<Function> createInstance(int ext_level
                                                  , std::list<ArgNameTypeRec> const& args
                                                  , std::map<std::string, Variable const> const& extvars
                                                  , bool has_void_returns);

        static void write_decls();
        static void write_impls();
    protected:
        Function(int ext_level
               , std::list<ArgNameTypeRec> const& args
               , std::map<std::string, Variable const> const& ext_vars)
            : _symbols(ext_level, args, ext_vars)
        {}
    protected:
        Function(Function const&) = delete;
    private:
        std::list<util::sref<MediateBase>> _candidate_paths;
        SymbolTable _symbols;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
