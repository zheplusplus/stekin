#include "function.h"
#include "node-base.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void Function::compile(util::sref<flchk::Filter> filter) const
{
    filter->defFunc(pos
                  , name
                  , param_names
                  , std::move(body.compile(std::move(util::mkmptr(new flchk::Filter)))));
}
