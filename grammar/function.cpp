#include "function.h"
#include "node-base.h"
#include "../flowcheck/filter.h"
#include "../flowcheck/node-base.h"
#include "../flowcheck/function.h"
#include "../proto/node-base.h"

using namespace grammar;

void function::compile(util::sref<flchk::filter> filter) const
{
    filter->def_func(pos
                   , name
                   , param_names
                   , std::move(body.compile(std::move(util::mkmptr(new flchk::filter)))));
}
