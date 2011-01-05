#ifndef __STACKENING_INSTNACE_INSTANCE_SCOPE_H__
#define __STACKENING_INSTNACE_INSTANCE_SCOPE_H__

#include <string>

namespace inst {

    struct type;
    struct variable;
    struct operation;
    struct symbol_table;
    struct function;

    struct scope {
        scope(function* func, symbol_table* symbols)
            : _func(func)
            , _symbols(symbols)
        {}
    public:
        void set_return_type(int lineno, type const* type) const;

        variable def_var(int lineno, type const* vtype, std::string const& name) const;
        variable query_var(int lineno, std::string const& name) const;

        operation const* query_binary(int lineno
                                    , std::string const& op
                                    , type const* lhs
                                    , type const* rhs) const;
        operation const* query_pre_unary(int lineno, std::string const& op, type const* rhs) const;
    public:
        symbol_table* get_symbols() const;
    private:
        function* const _func;
        symbol_table* const _symbols;
    };

}

#endif /* __STACKENING_INSTNACE_INSTANCE_SCOPE_H__ */
