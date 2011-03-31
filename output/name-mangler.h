#ifndef __STACKENING_OUTPUT_NAME_MANGLER_H__
#define __STACKENING_OUTPUT_NAME_MANGLER_H__

#include <string>
#include <vector>

#include "../util/pointer.h"

namespace output {

    std::string form_func_name(util::id func_name);
    std::string form_type(std::string const& type);
    std::string form_func_reference_type(int size);

}

#endif /* __STACKENING_OUTPUT_NAME_MANGLER_H__ */
