#include "global-scope.h"
#include "expr-nodes.h"
#include "inst-mediates.h"
#include "function.h"
#include "../instance/node-base.h"
#include "../instance/expr-nodes.h"
#include "../instance/block.h"
#include "../output/statement-writer.h"

namespace {

    struct WriteStmtInst
        : public inst::Statement
    {
        explicit WriteStmtInst(util::sptr<inst::Expression const> e)
            : expr(std::move(e))
        {}

        void write() const
        {
            output::beginWriteStmt();
            expr->write();
            output::endWriteStmt();
            output::endOfStatement();
        }

        util::sptr<inst::Expression const> const expr;
    };

    struct WriteStmtProto
        : public proto::Statement
    {
        WriteStmtProto()
            : proto::Statement(misc::position(0))
            , ref(misc::position(0), "value to write")
        {}

        util::sptr<inst::MediateBase> inst(util::sref<inst::Scope> inst_scope) const
        {
            return std::move(
                    util::mkmptr(new proto::DirectInst(std::move(
                                util::mkptr(new WriteStmtInst(std::move(ref.inst(inst_scope))))))));
        }

        proto::Reference const ref;
    };

}

using namespace proto;

GlobalScope::GlobalScope()
{
    util::sref<proto::Function> func_write = declare(misc::position(0)
                                                   , "write"
                                                   , std::vector<std::string>({ "value to write" })
                                                   , true);
    func_write->addStmt(std::move(util::mkptr(new WriteStmtProto)));
}
