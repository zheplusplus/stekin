#ifndef __STEKIN_FLOWCHECK_BUILT_IN_H__
#define __STEKIN_FLOWCHECK_BUILT_IN_H__

#include "filter.h"
#include "function.h"
#include "node-base.h"

namespace flchk {

    struct WriterExpr
        : public Expression
    {
        WriterExpr()
            : Expression(misc::position())
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::Block>
                                                  , util::sref<SymbolTable>) const;
        std::string typeName() const;
        bool isLiteral() const;
        bool boolValue() const;
        util::sptr<Expression const> fold() const;
    };

    struct WriterFunction
        : public Function
    {
        WriterFunction(util::sref<SymbolTable> global_symbols)
            : Function(misc::position()
                     , "write"
                     , std::vector<std::string>({ "value to write" })
                     , _mkBody(global_symbols))
        {}
    private:
        static util::sptr<Filter> _mkBody(util::sref<SymbolTable> global_symbols);
    };

    struct SelectorFunction
        : public Function
    {
        SelectorFunction(util::sref<SymbolTable> global_symbols)
            : Function(misc::position()
                     , "ifte"
                     , std::vector<std::string>({ "p", "c", "a" })
                     , _mkBody(global_symbols))
        {}
    private:
        static util::sptr<Filter> _mkBody(util::sref<SymbolTable> global_symbols);
    };

}

#endif /* __STEKIN_FLOWCHECK_BUILT_IN_H__ */
