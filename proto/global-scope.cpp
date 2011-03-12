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
            : proto::stmt_base(misc::pos_type(0))
            , ref(misc::pos_type(0), "value to write")
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
    util::sref<proto::function> func_write
                = declare(misc::pos_type(0), "write", std::vector<std::string>({ "value to write" }), true);
    func_write->add_stmt(std::move(util::mkptr(new write_stmt_proto)));
}
