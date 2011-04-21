#include <algorithm>
#include <sstream>

#include "name-mangler.h"
#include "../util/string.h"

std::string output::form_func_name(util::id func_addr)
{
    return "_stk_func_template_" + func_addr.str();
}

std::string output::form_type(std::string const& type_name)
{
    return "_stk_type_" + type_name;
}

std::string output::form_FuncReferenceType(int size)
{
    return "_stk_composite<" + util::str(size) + '>';
}
