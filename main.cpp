#include <map>
#include <list>

#include "parser/yy-misc.h"
#include "grammar/clause-builder.h"
#include "flowcheck/filter.h"
#include "flowcheck/node-base.h"
#include "flowcheck/function.h"
#include "proto/global-scope.h"
#include "proto/inst-mediates.h"
#include "proto/function.h"
#include "instance/node-base.h"
#include "instance/function.h"
#include "output/func-writer.h"
#include "util/pointer.h"
#include "report/errors.h"

namespace {
    struct CompileFail {};
}

static flchk::Block frontEnd()
{
    yyparse();
    if (error::hasError()) {
        throw CompileFail();
    }

    flchk::Block global_flow(std::move(parser::builder.buildAndClear()));
    if (error::hasError()) {
        throw CompileFail();
    }
    return std::move(global_flow);
}

static util::id semantic(flchk::Block global_flow)
{
    util::sptr<proto::Scope> proto_global_scope(new proto::GlobalScope);
    global_flow.compile(*proto_global_scope);
    if (error::hasError()) {
        throw CompileFail();
    }

    util::sref<inst::Function> inst_global_func(
                    inst::Function::createInstance(0
                                                 , std::list<inst::ArgNameTypeRec>()
                                                 , std::map<std::string, inst::Variable const>()
                                                 , true));
    proto::BlockMediate mediate(proto_global_scope->getStmts(), inst_global_func);
    inst_global_func->instNextPath();
    inst_global_func->addStmt(std::move(mediate.inst(inst_global_func)));
    if (error::hasError()) {
        throw CompileFail();
    }
    return inst_global_func.id();
}

static void outputAll(util::id global_func_id)
{
    inst::Function::writeDecls();
    output::writeMainBegin();
    output::stknMainFunc(global_func_id);
    output::writeMainEnd();
    inst::Function::writeImpls();
}

int main()
{
    try {
        outputAll(semantic(std::move(frontEnd())));
        return 0;
    } catch (CompileFail) {
        return 1;
    }
}
