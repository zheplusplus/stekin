#ifndef __STEKIN_INSTANCE_FUNCTION_H__
#define __STEKIN_INSTANCE_FUNCTION_H__

#include <string>
#include <list>

#include "fwd-decl.h"
#include "types.h"
#include "address.h"
#include "../util/sn.h"
#include "../util/pointer.h"

namespace inst {

    struct Function {
        struct ParamInfo {
            util::sptr<Type const> type;
            Address const address;

            ParamInfo(util::sptr<Type const> t, Address a)
                : type(std::move(t))
                , address(a)
            {}

            ParamInfo(ParamInfo&& rhs)
                : type(std::move(rhs.type))
                , address(rhs.address)
            {}
        };

        Function(util::sptr<Type const> rt
               , int l
               , int ss
               , std::list<ParamInfo> p
               , util::serial_num c
               , util::sptr<Statement const> b)
            : return_type(std::move(rt))
            , level(l)
            , stack_size(ss)
            , params(std::move(p))
            , call_sn(c)
            , body(std::move(b))
        {}

        void writeDecl() const;
        void writeImpl() const;

        util::sptr<Type const> const return_type;
        int const level;
        int const stack_size;
        std::list<ParamInfo> const params;
        util::serial_num const call_sn;
        util::sptr<Statement const> const body;
    };

}

#endif /* __STEKIN_INSTANCE_FUNCTION_H__ */
