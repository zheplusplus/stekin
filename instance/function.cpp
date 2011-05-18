#include "function.h"
#include "node-base.h"

using namespace inst;

void Function::writeDecl() const
{
      output::writeFuncDecl(type_exported_name, call_id, args, level, stack_size);
}

void Function::writeImpl() const
{
      output::writeFuncImpl(type_exported_name, call_id);
      body->write();
}
