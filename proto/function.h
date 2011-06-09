#ifndef __STEKIN_PROTO_FUNCTION_H__
#define __STEKIN_PROTO_FUNCTION_H__

#include <vector>
#include <list>
#include <map>

#include "fwd-decl.h"
#include "block.h"
#include "func-inst-draft.h"
#include "func-reference-type.h"
#include "../misc/pos-type.h"

namespace proto {

    struct Function {
        util::sref<FuncInstDraft> inst(misc::position const& pos
                                     , util::sref<SymbolTable const> ext_st
                                     , std::vector<util::sref<Type const>> const& arg_types);
        util::sref<FuncInstDraft> inst(int level
                                     , std::map<std::string, Variable const> const& ext_vars
                                     , std::vector<util::sref<Type const>> const& arg_types);

        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , bool func_hint_void_return)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , hint_void_return(func_hint_void_return)
        {}

        Function(Function const&) = delete;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
        bool hint_void_return;

        void setFreeVariables(std::vector<std::string> const& free_vars);
        std::map<std::string, Variable const> bindExternalVars(
                                                misc::position const& pos
                                              , util::sref<SymbolTable const> ext_st) const;

        util::sref<FuncReferenceType const> refType(misc::position const& reference_pos
                                                  , util::sref<SymbolTable const> st);
        std::vector<util::sptr<inst::Function const>> deliverFuncs();

        util::sref<Block> block();
    private:
        struct DraftInfo {
            std::map<std::string, Variable const> const ext_vars;
            std::vector<util::sref<Type const>> const arg_types;
            util::sptr<FuncInstDraft> draft;

            DraftInfo(std::map<std::string, Variable const> const& e
                    , std::vector<util::sref<Type const>> const& a
                    , util::sptr<FuncInstDraft> d)
                : ext_vars(e)
                , arg_types(a)
                , draft(std::move(d))
            {}

            DraftInfo(DraftInfo&& rhs)
                : ext_vars(rhs.ext_vars)
                , arg_types(rhs.arg_types)
                , draft(std::move(rhs.draft))
            {}
        };

        struct DraftCache
            : protected std::list<DraftInfo>
        {
            DraftCache() = default;
            ~DraftCache() = default;

            typedef std::list<DraftInfo> BaseType;
            typedef BaseType::const_iterator Iterator;

            Iterator find(std::map<std::string, Variable const> const& ext_vars
                        , std::vector<util::sref<Type const>> const& arg_types) const;
            Iterator end() const;
            void append(DraftInfo info);

            std::vector<util::sptr<inst::Function const>> deliverFuncs();
        };
    private:
        util::sref<FuncInstDraft> _draftInCacheOrNulIfNonexist(
                                std::map<std::string, Variable const> const& ext_vars
                              , std::vector<util::sref<Type const>> const& arg_types) const;
    private:
        DraftCache _draft_cache;
        std::vector<util::sptr<FuncReferenceType const>> _reference_types;
        std::vector<std::string> _free_variables;
        Block _block;
    };

}

#endif /* __STEKIN_PROTO_FUNCTION_H__ */
