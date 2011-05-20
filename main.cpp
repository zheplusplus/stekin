#include <map>
#include <list>
#include <algorithm>
#include <vector>

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
#include "proto/func-reference-type.h"
#include "instance/node-base.h"
#include "output/func-writer.h"
#include "util/pointer.h"
#include "report/errors.h"
#include "inspect/trace.h"

namespace {

    struct CompileFailure {};

    struct Functions {
        util::id const main_id;
        std::vector<util::sptr<inst::Function const>> funcs;

        Functions(util::id mid, std::vector<util::sptr<inst::Function const>> f)
            : main_id(mid)
            , funcs(std::move(f))
        {}

        Functions(Functions&& rhs)
            : main_id(rhs.main_id)
            , funcs(std::move(rhs.funcs))
        {}
    };

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

static Functions semantic(util::sptr<flchk::Filter> global_flow)
{
    util::sptr<proto::Scope> proto_global_scope(new proto::Scope);
    global_flow->compile(*proto_global_scope);
    if (error::hasError()) {
        throw CompileFailure();
    }

    proto::SymbolTable st;
    util::sptr<proto::FuncInstDraft> inst_global_func(proto::FuncInstDraft::createGlobal());
    util::sptr<proto::Block> global_block(proto_global_scope->deliver());
    inst_global_func->instantiate(*global_block);
    if (error::hasError()) {
        throw CompileFailure();
    }
    std::vector<util::sptr<inst::Function const>> funcs(global_block->deliverFuncs());
    funcs.push_back(inst_global_func->deliver());
    return Functions(inst_global_func.id(), std::move(funcs));
}

static void outputAll(Functions funcs)
{
    std::for_each(funcs.funcs.begin()
                , funcs.funcs.end()
                , [&](util::sptr<inst::Function const> const& func)
                  {
                      func->writeDecl();
                  });
    output::writeMainBegin();
    output::stknMainFunc(funcs.main_id);
    output::writeMainEnd();
    std::for_each(funcs.funcs.begin()
                , funcs.funcs.end()
                , [&](util::sptr<inst::Function const> const& func)
                  {
                      func->writeImpl();
                  });
}

int main()
{
    inspect::prepare_for_trace();
    try {
        outputAll(semantic(frontEnd()));
        return 0;
    } catch (CompileFailure) {
        return 1;
    }
}
