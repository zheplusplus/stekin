#include "global-filter.h"
#include "function.h"
#include "node-base.h"
#include "../proto/function.h"
#include "../proto/built-in.h"

using namespace flchk;

namespace {

    struct WriteExpr
        : public Expression
    {
        WriteExpr()
            : Expression(misc::position(0))
        {}

        util::sptr<proto::Expression const> compile(util::sref<proto::Scope>
                                                  , util::sref<SymbolTable>) const
        {
            return util::mkptr(new proto::WriteExpr);
        }

        std::string typeName() const
        {
            return "Stekin Built-in Write";
        }

        bool isLiteral() const
        {
            return false;
        }

        bool boolValue() const
        {
            return false;
        }

        util::sptr<Expression const> fold() const
        {
            return util::mkptr(new WriteExpr);
        }
    };

    struct WriteFunction
        : public Function
    {
        static util::sref<Function> getInstance(util::sref<SymbolTable> global_symbols)
        {
            static WriteFunction w(global_symbols);
            return util::mkref(w);
        }
    private:
        WriteFunction(util::sref<SymbolTable> global_symbols)
            : Function(misc::position(0)
                     , "write"
                     , std::vector<std::string>({ "value to write" })
                     , mkBody(global_symbols))
        {}

        static util::sptr<Filter> mkBody(util::sref<SymbolTable> global_symbols)
        {
            util::sptr<Filter> body(new FuncBodyFilter(global_symbols));
            body->addArith(misc::position(0), util::mkptr(new WriteExpr));
            return std::move(body);
        }
    };

}

GlobalFilter::GlobalFilter()
{
    _symbols.defFunc(WriteFunction::getInstance(util::mkref(_symbols)));
}
