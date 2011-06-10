#ifndef __STEKIN_OUTPUT_NAME_MANGLER_H__
#define __STEKIN_OUTPUT_NAME_MANGLER_H__

#include <string>
#include <vector>

#include "../util/sn.h"

namespace output {

    std::string formFuncName(util::serial_num func_sn);
    std::string formType(std::string const& type);
    std::string formFuncReferenceType(int size);

}

#endif /* __STEKIN_OUTPUT_NAME_MANGLER_H__ */
