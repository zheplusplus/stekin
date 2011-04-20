#ifndef __STEKIN_FLOW_CHECK_FUNCTION_H__
#define __STEKIN_FLOW_CHECK_FUNCTION_H__

#include <string>
#include <vector>

#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct function {
        function(misc::pos_type const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , block func_body
               , bool func_contains_void_return)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , body(std::move(func_body))
            , contains_void_return(func_contains_void_return)
        {}

        void compile(util::sref<proto::scope> scope) const;
        util::sref<proto::function> declare(util::sref<proto::scope> scope) const;

        misc::pos_type const pos;
        std::string const name;
        std::vector<std::string> const param_names;
        block const body;
        bool const contains_void_return;
    };

}

#endif /* __STEKIN_FLOW_CHECK_FUNCTION_H__ */
