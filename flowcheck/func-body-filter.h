#ifndef __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__
#define __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__

#include "filter.h"

namespace flchk {

    struct FuncBodyFilter
        : public Filter
    {
        FuncBodyFilter() = default;
    public:
        void defVar(misc::position const& pos
                  , std::string const& name
                  , util::sptr<Expression const>);
        util::sptr<Expression const> makeRef(misc::position const& pos, std::string const& name);
        util::sptr<Expression const> makeCall(misc::position const& pos
                                            , std::string const& name
                                            , std::vector<util::sptr<Expression const>> args);
        util::sptr<Expression const> makeFuncReference(misc::position const& pos
                                                     , std::string const& name
                                                     , int param_count);
    protected:
        void _defFunc(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const&
                    , Accumulator body);
    };

}

#endif /* __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__ */
