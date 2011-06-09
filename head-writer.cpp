#include <iostream>
#include <string>

#include "util/string.h"
#include "misc/platform.h"

std::string const HEAD_TEMPLATE(
"#include <algorithm>\n"
"#include <iostream>\n"
"\n"
"typedef $INT_TYPE_NAME _stk_type_int;\n"
"typedef $FLOAT_TYPE_NAME _stk_type_float;\n"
"typedef $BOOLEAN_TYPE_NAME _stk_type_1_byte;\n"
"\n"
);

int main()
{
    std::cout <<
        util::replace_all(
        util::replace_all(
        util::replace_all(
            HEAD_TEMPLATE
                , "$INT_TYPE_NAME", platform::int_traits::type_name())
                , "$FLOAT_TYPE_NAME", platform::float_traits::type_name())
                , "$BOOLEAN_TYPE_NAME", platform::bool_traits::type_name())
    ;
    return 0;
}
