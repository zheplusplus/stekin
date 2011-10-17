#include <iostream>

#include "stmt-writer.h"
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

void output::initThisLevel(int offset, std::string const& type_exported_name)
{
    std::cout << "new(" << offset << " + (char*)(_stk_bases.this_base()))" << type_exported_name;
}

void output::addResEntry(int entry_offset)
{
    std::cout << "_res_entries.add(" << entry_offset << ");";
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
