#include <algorithm>
#include <sstream>

#include "name-mangler.h"
#include "../util/string.h"

std::string output::formFuncName(util::serial_num func_sn)
{
    return "_stk_func_template_" + util::str(func_sn.n);
}

std::string output::formType(std::string const& type_name)
{
    return "_stk_type_" + type_name;
}

std::string output::formFuncReferenceType(int size)
{
    return "_stk_composite<" + util::str(size) + '>';
}
