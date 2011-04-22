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

    util::sptr<proto::Scope> proto_GlobalScope(new proto::GlobalScope);
    global_flow.compile(*proto_GlobalScope);
    if (error::hasError()) {
        return 1;
    }

    util::sref<inst::Function>
            inst_global_func(inst::Function::create_instance(0
                                                           , std::list<inst::arg_name_type_pair>()
                                                           , std::map<std::string, inst::variable const>()
                                                           , true));
    proto::BlockMediate mediate(proto_GlobalScope->getStmts(), inst_global_func);
    inst_global_func->inst_next_path();
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
