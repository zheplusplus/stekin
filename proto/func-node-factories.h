#ifndef __STACKENING_PROTO_FUNCTION_NODE_FACTORIES_H__
#define __STACKENING_PROTO_FUNCTION_NODE_FACTORIES_H__

#include <list>

#include "symbol-table.h"
#include "node-factory.h"
#include "scope.h"

namespace proto {

    struct func_node_sub_factory;

    struct func_node_factory
        : public node_factory
    {
        explicit func_node_factory(symbol_table* symbols)
            : _symbols(symbols)
        {}

        func_node_factory(func_node_factory&& rhs)
            : _symbols(rhs._symbols)
        {}
    public:
        scope const* create_branch_scope();
        scope const* create_loop_scope();
    protected:
        func_node_sub_factory* _create_sub_factory();

        symbol_table* const _symbols;
        std::list<func_node_sub_factory> _sub_factories;
    };

    struct func_node_sub_factory
        : public func_node_factory
    {
        explicit func_node_sub_factory(symbol_table* symbols)
            : func_node_factory(symbols)
        {}

        func_node_sub_factory(func_node_sub_factory&& rhs)
            : func_node_factory(std::move(rhs))
        {}
    public:
        var_def const* def_var(misc::pos_type const& pos, std::string const& name, expr_base const*);
    };

}

#endif /* __STACKENING_PROTO_FUNCTION_NODE_FACTORIES_H__ */
