#include <iostream>

#include <util/str-comprehension.h>

#include "expr-writer.h"
#include "name-mangler.h"

void output::writeInt(platform::int_type i)
{
    std::cout << "_stk_type_int(" << i << ")";
}

void output::writeFloat(platform::float_type d)
{
    std::cout << "_stk_type_float(" << d << ")";
}

void output::writeBool(bool b)
{
    std::cout << "_stk_type_bool(" << b << ")";
}

void output::writeString(char const* value, int length)
{
    std::cout << "_stk_type_string(" + util::cstr_repr(value, length) << "," << length << ")";
}

void output::refLevel(int offset, int level, std::string const& type_exported_name)
{
    std::cout << "(*(" << type_exported_name << "*)"
                 "(" << offset << " + (char*)(_stk_bases._stk_ext_bases[" << level << "])))";
}

void output::writeOperator(std::string const& op_img)
{
    std::cout << " " << op_img << " ";
}

void output::emptyList()
{
    std::cout << emptyListType() << "()";
}

void output::listBegin(int size, std::string const& member_type_exported_name)
{
    std::cout << "_stk_list_builder<" << size << ", " << member_type_exported_name << " >(";
}

void output::listNextMember()
{
    std::cout << ").push(";
}

void output::listEnd()
{
    std::cout << ").build()";
}

void output::memberCallBegin(std::string const& member_name)
{
    std::cout << '.' << member_name << '(';
}

void output::memberCallEnd()
{
    std::cout << ')';
}

void output::listAppendBegin()
{
    std::cout << "_stk_list_append(";
}

void output::listAppendEnd()
{
    std::cout << ')';
}

void output::beginExpr()
{
    std::cout << "(";
}

void output::endExpr()
{
    std::cout << ")";
}
