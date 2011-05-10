#ifndef __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__
#define __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__

#include <map>
#include <list>
#include <string>

#include "fwd-decl.h"
#include "../instance/block.h"
#include "../misc/pos-type.h"

namespace proto {

    struct FuncInstDraft {
        virtual ~FuncInstDraft() {}
    public:
        virtual void setReturnType(misc::position const& pos, util::sref<Type const> return_type);
        virtual util::sref<Type const> getReturnType() const;
        virtual bool isReturnTypeResolved() const;

        void addPath(util::sref<Statement> path);
        void instNextPath();
        bool hasMorePath() const;

        void instantiate(util::sref<Statement> stmt);
    public:
        static util::sref<FuncInstDraft> create(util::sref<SymbolTable> st, bool has_void_returns);
        static util::sref<FuncInstDraft> badDraft();

        static void writeDecls();
        static void writeImpls();
    protected:
        explicit FuncInstDraft(util::sref<SymbolTable> st)
            : _symbols_or_nul_if_inst_done(st)
            , _level(0)
            , _stack_size(0)
        {}

        FuncInstDraft(FuncInstDraft const&) = delete;
    protected:
        void _setSymbolInfo();
        void _addStmt(util::sptr<inst::Statement const> stmt);
    private:
        inst::Block _block;
        std::list<util::sref<Statement>> _candidate_paths;
        util::sref<SymbolTable> _symbols_or_nul_if_inst_done;
        int _level;
        int _stack_size;
        std::list<Variable> _args;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__ */
