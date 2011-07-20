#include <algorithm>

#include "function.h"
#include "node-base.h"
#include "../output/func-writer.h"

using namespace inst;

static std::list<output::StackVarRec> toStackVars(std::list<Function::ParamInfo> const& params)
{
    std::list<output::StackVarRec> recs;
    std::for_each(params.begin()
                , params.end()
                , [&](Function::ParamInfo const& param)
                  {
                      recs.push_back(output::StackVarRec(param.type->exportedName()
                                                       , param.address.offset
                                                       , param.address.level));
                  });
    return std::move(recs);
}

void Function::writeDecl() const
{
      output::writeFuncDecl(return_type->exportedName()
                          , call_sn
                          , toStackVars(params)
                          , level
                          , stack_size);
}

void Function::writeImpl() const
{
      output::writeFuncImpl(return_type->exportedName(), call_sn);
      body->write();
}
