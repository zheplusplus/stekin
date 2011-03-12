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
    if (error::has_error()) {
        return 1;
    }

    flchk::block global_flow(std::move(parser::builder.build_and_clear()));
    if (error::has_error()) {
        return 1;
    }

    util::sptr<proto::scope> proto_global_scope(new proto::global_scope);
    global_flow.compile(*proto_global_scope);
    if (error::has_error()) {
        return 1;
    }

    util::sref<inst::function>
            inst_global_func(inst::function::create_instance(0
                                                           , std::list<inst::arg_name_type_pair>()
                                                           , std::map<std::string, inst::variable const>()
                                                           , true));
    proto::block_mediate mediate(proto_global_scope->get_stmts(), inst_global_func);
    inst_global_func->inst_next_path();
    inst_global_func->add_stmt(std::move(mediate.inst(inst_global_func)));
    if (error::has_error()) {
        return 1;
    }

    inst::function::write_decls();
    output::write_main_begin();
    output::stk_main_func(inst_global_func.id());
    output::write_main_end();
    inst::function::write_impls();
    return 0;
}
