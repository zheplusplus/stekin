#ifndef __STEKIN_FLOWCHECK_SYMBOL_DEFINITION_FILTER_H__
#define __STEKIN_FLOWCHECK_SYMBOL_DEFINITION_FILTER_H__

#include "filter.h"

namespace flchk {

    struct SymbolDefFilter
        : public Filter
    {
        SymbolDefFilter() = default;
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
                    , Accumulator);
    };

}

#endif /* __STEKIN_FLOWCHECK_SYMBOL_DEFINITION_FILTER_H__ */
