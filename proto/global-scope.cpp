#include "global-scope.h"
#include "expr-nodes.h"
#include "inst-mediates.h"
#include "function.h"
#include "../instance/node-base.h"
#include "../instance/expr-nodes.h"
#include "../instance/block.h"
#include "../output/statement-writer.h"

namespace {

    struct write_stmt_inst
        : public inst::Statement
    {
        explicit write_stmt_inst(util::sptr<inst::Expression const> e)
            : expr(std::move(e))
        {}

        void write() const
        {
            output::begin_write_stmt();
            expr->write();
            output::end_write_stmt();
            output::end_of_statement();
        }

        util::sptr<inst::Expression const> const expr;
    };

    struct write_stmt_proto
        : public proto::Statement
    {
        write_stmt_proto()
            : proto::Statement(misc::position(0))
            , ref(misc::position(0), "value to write")
        {}

        util::sptr<inst::mediate_base> inst(util::sref<inst::scope> inst_scope) const
        {
            return std::move(
                    util::mkmptr(new proto::direct_inst(std::move(
                                util::mkptr(new write_stmt_inst(std::move(ref.inst(inst_scope))))))));
        }

        proto::reference const ref;
    };

}

using namespace proto;

global_scope::global_scope()
{
    util::sref<proto::Function> func_write
                = declare(misc::position(0), "write", std::vector<std::string>({ "value to write" }), true);
    func_write->addStmt(std::move(util::mkptr(new write_stmt_proto)));
}
