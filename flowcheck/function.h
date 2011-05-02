#ifndef __STEKIN_FLOWCHECK_FUNCTION_H__
#define __STEKIN_FLOWCHECK_FUNCTION_H__

#include <string>
#include <vector>

#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct Function {
        Function(misc::position const& ps
               , std::string const& func_name
               , std::vector<std::string> const& params
               , util::sptr<Filter> func_body)
            : pos(ps)
            , name(func_name)
            , param_names(params)
            , _body(std::move(func_body))
            , _func_proto(NULL)
        {}

        Function(Function&& rhs)
            : pos(rhs.pos)
            , name(rhs.name)
            , param_names(rhs.param_names)
            , _body(std::move(rhs._body))
            , _func_proto(rhs._func_proto)
        {}

        util::sref<proto::Function> compile(util::sref<proto::Scope> scope);
        std::vector<std::string> freeVariables() const;

        misc::position const pos;
        std::string const name;
        std::vector<std::string> const param_names;
    private:
        util::sptr<Filter> _body;
        util::sref<proto::Function> _func_proto;
    };

}

#endif /* __STEKIN_FLOWCHECK_FUNCTION_H__ */
