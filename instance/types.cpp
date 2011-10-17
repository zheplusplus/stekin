#include <output/name-mangler.h>
#include <output/stmt-writer.h>

#include "types.h"

using namespace inst;

void Type::writeResEntry(int) const {}

util::sptr<output::StackVarRec const> Type::makeParameter(Address const& addr) const
{
    return util::mkptr(new output::Parameter(exportedName(), addr.offset, addr.level));
}

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

std::string EmptyListType::exportedName() const
{
    return output::emptyListType();
}

std::string ListType::exportedName() const
{
    return output::formListType(member_type->exportedName());
}

util::sptr<output::StackVarRec const> ListType::makeParameter(Address const& addr) const
{
    return util::mkptr(new output::ResourceParam(exportedName(), addr.offset, addr.level));
}

std::string ClosureType::exportedName() const
{
    return output::formFuncReferenceType(size);
}

void ListType::writeResEntry(int offset) const
{
    output::addResEntry(offset);
}
