#ifndef __STEKIN_INSTANCE_FUNCTION_H__
#define __STEKIN_INSTANCE_FUNCTION_H__

#include <string>
#include <list>

#include "fwd-decl.h"
#include "../output/func-writer.h"
#include "../util/sn.h"
#include "../util/pointer.h"

namespace inst {

    struct Function {
        Function(std::string const& en
               , int l
               , int ss
               , std::list<output::StackVarRec> const& a
               , util::serial_num c
               , util::sptr<Statement const> b)
            : type_exported_name(en)
            , level(l)
            , stack_size(ss)
            , args(a)
            , call_sn(c)
            , body(std::move(b))
        {}

        void writeDecl() const;
        void writeImpl() const;

        std::string const type_exported_name;
        int const level;
        int const stack_size;
        std::list<output::StackVarRec> const args;
        util::serial_num const call_sn;
        util::sptr<Statement const> const body;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
