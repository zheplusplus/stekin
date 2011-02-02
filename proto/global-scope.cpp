#include "scope.h"
#include "expr-nodes.h"
#include "stmt-nodes.h"
#include "func-templ.h"
#include "symbol-table.h"
#include "inst-mediates.h"
#include "../instance/node-base.h"
#include "../instance/expr-nodes.h"
#include "../output/statement-writer.h"

namespace {

    struct proto_global_scope
        : public proto::scope
    {
        proto_global_scope();
    private:
        proto::symbol_table _symbols;
    };

    struct write_stmt_inst
        : public inst::stmt_base
    {
        explicit write_stmt_inst(util::sptr<inst::expr_base const> e)
            : expr(std::move(e))
        {}

        void write() const
        {
            output::begin_write_stmt();
            expr->write();
            output::end_write_stmt();
            output::end_of_statement();
        }

        util::sptr<inst::expr_base const> const expr;
    };

    struct write_stmt_proto
        : public proto::stmt_base
    {
        write_stmt_proto()
            : ref(misc::pos_type(0), "value to write")
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> inst_scope) const
        {
            return std::move(
                    util::mkmptr(new proto::direct_inst(std::move(
                                util::mkptr(new write_stmt_inst(std::move(ref.inst(inst_scope))))))));
        }

        proto::reference const ref;
    };

    proto_global_scope::proto_global_scope()
        : proto::scope(util::mkref(_symbols))
    {
        util::sref<proto::func_templ> func_write
                    = decl_func(misc::pos_type(0), "write", std::vector<std::string>({ "value to write" }));
        func_write->get_scope()->add_custom_statement(std::move(util::mkptr(new write_stmt_proto)));
    }

}

using namespace proto;

util::sptr<scope> scope::global_scope()
{
    return std::move(util::mkmptr(new proto_global_scope));
}

void scope::add_custom_statement(util::sptr<stmt_base const> stmt)
{
    _block.add_stmt(std::move(stmt));
}
