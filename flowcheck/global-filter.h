#ifndef __STEKIN_FLOWCHECK_GLOBAL_FILTER_H__
#define __STEKIN_FLOWCHECK_GLOBAL_FILTER_H__

#include "func-body-filter.h"
#include "built-in.h"

namespace flchk {

    struct GlobalFilter
        : public FuncBodyFilter
    {
        GlobalFilter();
    private:
        WriterFunction _writer_func;
        SelectorFunction _selector_func;
    };

}

#endif /* __STEKIN_FLOWCHECK_GLOBAL_FILTER_H__ */
