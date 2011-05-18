#ifndef __STEKIN_INSTANCE_FUNCTION_H__
#define __STEKIN_INSTANCE_FUNCTION_H__

#include <string>
#include <list>

#include "fwd-decl.h"
#include "../output/func-writer.h"

namespace inst {

    struct Function {
        Function(std::string const& en
               , int l
               , int ss
               , std::list<output::StackVarRec> const& a
               , util::id cid
               , util::sptr<Statement const> b)
            : type_exported_name(en)
            , level(l)
            , stack_size(ss)
            , args(a)
            , call_id(cid)
            , body(std::move(b))
        {}

        void writeDecl() const;
        void writeImpl() const;

        std::string const type_exported_name;
        int const level;
        int const stack_size;
        std::list<output::StackVarRec> const args;
        util::id const call_id;
        util::sptr<Statement const> const body;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
