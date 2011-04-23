#include <iostream>

#include "statement-writer.h"
#include "name-mangler.h"

void output::kwReturn()
{
    std::cout << "return ";
}

void output::returnNothing()
{
    kwReturn();
    std::cout << formType("void") << "()";
    endOfStatement();
}

void output::refThisLevel(int offset, std::string const& type_exported_name)
{
    std::cout << "(*(" << type_exported_name << "*)(" << offset << " + (char*)(_stk_bases.this_base())))";
}

void output::refLevel(int offset, int level, std::string const& type_exported_name)
{
    std::cout << "(*(" << type_exported_name << "*)"
                 "(" << offset << " + (char*)(_stk_bases._stk_ext_bases[" << level << "])))";
}

void output::signAssign()
{
    std::cout << " = ";
}

void output::beginExpr()
{
    std::cout << "(";
}

void output::endExpr()
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

void output::blockBegin()
{
    std::cout << "{" << std::endl;
}

void output::blockEnd()
{
    std::cout << "}" << std::endl;
}

void output::endOfStatement()
{
    std::cout << ";" << std::endl;
}

void output::writeInt(int i)
{
    std::cout << "(" << i << ")";
}

void output::writeFloat(double d)
{
    std::cout << "(" << d << ")";
}

void output::writeBool(bool b)
{
    std::cout << "(" << b << ")";
}

void output::writeOperator(std::string const& op_img)
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
