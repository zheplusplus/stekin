#ifndef __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__
#define __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__

#include <map>
#include <list>
#include <string>

#include "fwd-decl.h"
#include "symbol-table.h"
#include "../instance/function.h"
#include "../misc/pos-type.h"
#include "../util/sn.h"

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
        util::sptr<inst::Function const> deliver();
    public:
        static util::sptr<FuncInstDraft> create(int ext_lvl
                                              , std::list<ArgNameTypeRec> const& args
                                              , std::map<std::string, Variable const> const& extvars
                                              , bool has_void_returns);
        static util::sptr<FuncInstDraft> createGlobal();
        static util::sref<FuncInstDraft> badDraft();
    protected:
        FuncInstDraft(int ext_lvl
                    , std::list<ArgNameTypeRec> const& args
                    , std::map<std::string, Variable const> const& extvars)
            : sn(util::serial_num::next())
            , _inst_func_or_nul_if_not_inst(NULL)
            , _symbols(ext_lvl, args, extvars)
        {}

        FuncInstDraft()
            : sn(util::serial_num::next())
            , _inst_func_or_nul_if_not_inst(NULL)
        {}

        FuncInstDraft(FuncInstDraft const&) = delete;
    public:
        util::serial_num const sn;
    private:
        util::sptr<inst::Function> _inst_func_or_nul_if_not_inst;
        std::list<util::sref<Statement>> _candidate_paths;
        SymbolTable _symbols;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_INSTANTIATE_DRAFT_H__ */
