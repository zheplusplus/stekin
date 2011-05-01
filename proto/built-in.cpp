#include "built-in.h"
#include "inst-mediates.h"
#include "function.h"
#include "../instance/node-base.h"
#include "../instance/expr-nodes.h"
#include "../instance/block.h"
#include "../instance/type.h"
#include "../output/statement-writer.h"

using namespace proto;

namespace {

    struct WriteExprInst
        : public inst::Expression
    {
        explicit WriteExprInst(util::sptr<inst::Expression const> e)
            : expr(std::move(e))
        {}

        util::sref<inst::Type const> typeof() const
        {
            return inst::Type::BIT_VOID;
        }

        void write() const
        {
            output::beginWriteStmt();
            expr->write();
            output::endWriteStmt();
        }

        util::sptr<inst::Expression const> const expr;
    };

}

util::sptr<inst::Expression const> WriteExpr::inst(util::sref<inst::Scope> scope) const
{
    return util::mkptr(new WriteExprInst(ref.inst(scope)));
}
