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
        void setReturnType(misc::position const& pos, util::sref<Type const> return_type);
        virtual util::sref<Type const> getReturnType() const;
        virtual bool isReturnTypeResolved() const;

        void addPath(util::sref<MediateBase> path);
        void instNextPath();
        bool hasMorePath() const;

        void instantiate(util::sref<MediateBase> mediate);
    public:
        static util::sref<Function> createInstance(util::sref<SymbolTable> st
                                                 , bool has_void_returns);

        static void writeDecls();
        static void writeImpls();
    protected:
        explicit Function(util::sref<SymbolTable> st)
            : _symbols_or_nul_if_inst_done(st)
            , _level(0)
            , _stack_size(0)
        {}

        Function(Function const&) = delete;
    protected:
        void _setSymbolInfo();
    private:
        std::list<util::sref<MediateBase>> _candidate_paths;
        util::sref<SymbolTable> _symbols_or_nul_if_inst_done;
        int _level;
        int _stack_size;
        std::list<Variable> _args;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
