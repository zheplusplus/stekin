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

int main()
{
    yyparse();
    if (error::hasError()) {
        return 1;
    }

    flchk::Block global_flow(std::move(parser::builder.buildAndClear()));
    if (error::hasError()) {
        return 1;
    }

    util::sptr<proto::Scope> proto_global_scope(new proto::GlobalScope);
    global_flow.compile(*proto_global_scope);
    if (error::hasError()) {
        return 1;
    }

    util::sref<inst::Function>
            inst_global_func(inst::Function::createInstance(0
                                                          , std::list<inst::ArgNameTypeRec>()
                                                          , std::map<std::string, inst::Variable const>()
                                                          , true));
    proto::BlockMediate mediate(proto_global_scope->getStmts(), inst_global_func);
    inst_global_func->instNextPath();
    inst_global_func->addStmt(std::move(mediate.inst(inst_global_func)));
    if (error::hasError()) {
        return 1;
    }

    inst::Function::write_decls();
    output::write_main_begin();
    output::stk_main_func(inst_global_func.id());
    output::write_main_end();
    inst::Function::write_impls();
    return 0;
}
