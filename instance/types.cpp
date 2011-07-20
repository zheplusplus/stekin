#include "types.h"
#include "../output/name-mangler.h"
#include "../output/func-writer.h"

using namespace inst;

std::string VoidPrimitive::exportedName() const
{
    return output::formType("void");
}

std::string IntPrimitive::exportedName() const
{
    return output::formType("int");
}

std::string FloatPrimitive::exportedName() const
{
    return output::formType("float");
}

std::string BoolPrimitive::exportedName() const
{
    return output::formType("bool");
}

std::string ClosureType::exportedName() const
{
    return output::formFuncReferenceType(size);
}
