#include <iostream>

#include "statement-writer.h"
#include "name-mangler.h"

void output::return_kw()
{
    std::cout << "return ";
}

void output::return_nothing()
{
    return_kw();
    std::cout << form_type("void") << "()";
    end_of_statement();
}

void output::ref_this_level(int offset, std::string const& type_exported_name)
{
    std::cout << "(*(" << type_exported_name << "*)(" << offset << " + (char*)(_stk_bases.this_base())))";
}

void output::ref_level(int offset, int level, std::string const& type_exported_name)
{
    std::cout << "(*(" << type_exported_name << "*)"
                 "(" << offset << " + (char*)(_stk_bases._stk_ext_bases[" << level << "])))";
}

void output::assign_sign()
{
    std::cout << " = ";
}

void output::begin_expr()
{
    std::cout << "(";
}

void output::end_expr()
{
    std::cout << ")";
}

void output::branch_if()
{
    std::cout << "if ";
}

void output::branch_else()
{
    std::cout << " else ";
}

void output::Block_begin()
{
    std::cout << "{" << std::endl;
}

void output::Block_end()
{
    std::cout << "}" << std::endl;
}

void output::end_of_statement()
{
    std::cout << ";" << std::endl;
}

void output::write_int(int i)
{
    std::cout << "(" << i << ")";
}

void output::write_float(double d)
{
    std::cout << "(" << d << ")";
}

void output::write_bool(bool b)
{
    std::cout << "(" << b << ")";
}

void output::write_operator(std::string const& op_img)
{
    std::cout << " " << op_img << " ";
}

void output::begin_write_stmt()
{
    std::cout << "std::cout << ";
}

void output::end_write_stmt()
{
    std::cout << " << std::endl";
}
