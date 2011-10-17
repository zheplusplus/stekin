#include "global-filter.h"
#include "built-in.h"

using namespace flchk;

GlobalFilter::GlobalFilter()
    : _writer_func(util::mkref(_symbols))
    , _selector_func(util::mkref(_symbols))
{
    _symbols.defFunc(util::mkref(_writer_func));
    _symbols.defFunc(util::mkref(_selector_func));
}
