#include <algorithm>

#include <output/func-writer.h>

#include "function.h"
#include "node-base.h"

using namespace inst;

static std::vector<util::sptr<output::StackVarRec const>>
                toStackVars(std::list<Function::ParamInfo> const& params)
{
    std::vector<util::sptr<output::StackVarRec const>> recs;
    std::for_each(params.begin()
                , params.end()
                , [&](Function::ParamInfo const& param)
                  {
                      recs.push_back(param.type->makeParameter(param.address));
                  });
    return std::move(recs);
}

void Function::writeDecl() const
{
      output::writeFuncDecl(return_type->exportedName()
                          , call_sn
                          , toStackVars(params)
                          , level
                          , stack_size
                          , res_entries.size());
}

void Function::writeImpl() const
{
      output::writeFuncImpl(return_type->exportedName(), call_sn);
      body->write();
}
