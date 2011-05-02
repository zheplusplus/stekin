#ifndef __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__
#define __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__

#include "filter.h"
#include "symbol-table.h"

namespace flchk {

    struct FuncBodyFilter
        : public Filter
    {
        explicit FuncBodyFilter(util::sref<SymbolTable const> ext_symbols)
            : _symbols(ext_symbols)
        {}
    public:
        void defVar(misc::position const& pos
                  , std::string const& name
                  , util::sptr<Expression const>);
        util::sref<SymbolTable> getSymbols();
    protected:
        void _defFunc(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const&
                    , util::sptr<Filter> body);
        FuncBodyFilter() {}
    protected:
        SymbolTable _symbols;
    };

}

#endif /* __STEKIN_FLOWCHECK_FUNCTION_BODY_FILTER_H__ */
