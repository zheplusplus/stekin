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
    endOfStatement();
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

void output::branchIf()
{
    std::cout << "if ";
}

void output::branchElse()
{
    std::cout << " else ";
}

void output::block_begin()
{
    std::cout << "{" << std::endl;
}

void output::block_end()
{
    std::cout << "}" << std::endl;
}

void output::endOfStatement()
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

void output::beginWriteStmt()
{
    std::cout << "std::cout << ";
}

void output::endWriteStmt()
{
    std::cout << " << std::endl";
}
