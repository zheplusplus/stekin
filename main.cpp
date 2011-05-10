#include <map>
#include <list>

#include "parser/yy-misc.h"
#include "grammar/clause-builder.h"
#include "flowcheck/filter.h"
#include "flowcheck/node-base.h"
#include "flowcheck/function.h"
#include "proto/scope.h"
#include "proto/node-base.h"
#include "proto/function.h"
#include "proto/symbol-table.h"
#include "proto/func-inst-draft.h"
#include "proto/variable.h"
#include "proto/type.h"
#include "instance/node-base.h"
#include "output/func-writer.h"
#include "util/pointer.h"
#include "report/errors.h"

namespace {
    struct CompileFailure {};
}

static util::sptr<flchk::Filter> frontEnd()
{
    yyparse();
    if (error::hasError()) {
        throw CompileFailure();
    }

    util::sptr<flchk::Filter> global_flow(std::move(parser::builder.buildAndClear()));
    if (error::hasError()) {
        throw CompileFailure();
    }
    return std::move(global_flow);
}

static util::id semantic(util::sptr<flchk::Filter> global_flow)
{
    util::sptr<proto::Scope> proto_global_scope(new proto::Scope);
    global_flow->compile(*proto_global_scope);
    if (error::hasError()) {
        throw CompileFailure();
    }

    proto::SymbolTable st;
    util::sref<proto::FuncInstDraft> inst_global_func(
            proto::FuncInstDraft::create(util::mkref(st), true));
    inst_global_func->instantiate(*proto_global_scope->inst());
    if (error::hasError()) {
        throw CompileFailure();
    }
    return inst_global_func.id();
}

static void outputAll(util::id global_func_id)
{
    proto::FuncInstDraft::writeDecls();
    output::writeMainBegin();
    output::stknMainFunc(global_func_id);
    output::writeMainEnd();
    proto::FuncInstDraft::writeImpls();
}

int main()
{
    try {
        outputAll(semantic(frontEnd()));
        return 0;
    } catch (CompileFailure) {
        return 1;
    }
}
