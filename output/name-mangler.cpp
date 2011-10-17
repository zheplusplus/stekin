#include <util/string.h>

#include "name-mangler.h"

std::string output::formFuncName(util::serial_num func_sn)
{
    return "_stk_func_template_" + util::str(func_sn.n);
}

std::string output::formType(std::string const& type_name)
{
    return "_stk_type_" + type_name;
}

std::string output::formListType(std::string const& member_type_exported_name)
{
    return "_stk_list<" + member_type_exported_name + " >";
}

std::string output::emptyListType()
{
    return "_stk_empty_list_type";
}

std::string output::formFuncReferenceType(int size)
{
    return "_stk_composite<" + util::str(size) + '>';
}
