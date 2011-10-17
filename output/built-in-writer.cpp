#include <iostream>

#include "built-in-writer.h"

void output::beginWriterStmt()
{
    std::cout << "std::cout << ";
}

void output::endWriterStmt()
{
    std::cout << " << std::endl";
}
