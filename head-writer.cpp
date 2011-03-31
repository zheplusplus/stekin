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
                , "$INT_TYPE_NAME", platform::i4_traits::type_name())
                , "$FLOAT_TYPE_NAME", platform::f8_traits::type_name())
                , "$BOOLEAN_TYPE_NAME", platform::b1_traits::type_name())
    ;
    return 0;
}

std::string const& platform::c_long_long::type_name()
{
    static std::string const N("long long");
    return N;
}

std::string const& platform::c_long::type_name()
{
    static std::string const N("long");
    return N;
}

std::string const& platform::c_int::type_name()
{
    static std::string const N("int");
    return N;
}

std::string const& platform::c_short::type_name()
{
    static std::string const N("short");
    return N;
}

std::string const& platform::c_double::type_name()
{
    static std::string const N("double");
    return N;
}

std::string const& platform::c_char::type_name()
{
    static std::string const N("char");
    return N;
}
