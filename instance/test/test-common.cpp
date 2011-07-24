#include "test-common.h"
#include "../../misc/platform.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

DataTree& DataTree::operator()(NodeType const& type
                             , std::string const& name
                             , int level
                             , int stack_size_or_offset)
{
    BaseType::operator()(type, InstanceData(level, stack_size_or_offset), name);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type
                             , std::string const& name
                             , int level
                             , int stack_size_or_offset
                             , int self_offset)
{
    BaseType::operator()(type, InstanceData(level, stack_size_or_offset, self_offset), name);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, std::string const& name)
{
    BaseType::operator()(type, InstanceData(), name);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type)
{
    BaseType::operator()(type, InstanceData());
    return *this;
}

std::ostream& operator<<(std::ostream& os, InstanceData const& data)
{
    if (-1 != data.level) {
        os << " level=" << data.level;
    }
    if (-1 != data.stack_size_or_offset) {
        os << " stack_size or offset=" << data.stack_size_or_offset;
    }
    if (-1 != data.self_offset) {
        os << " self offset=" << data.self_offset;
    }
    return os;
}

NodeType const test::FUNC_DECL_BEGIN("func declaration begin");
NodeType const test::FUNC_DECL_END("func declaration end");
NodeType const test::FUNC_DEF("func definition");
NodeType const test::PARAMETER("parameter");

NodeType const test::BLOCK_BEGIN("block begin");
NodeType const test::BLOCK_END("block end");

NodeType const test::INTEGER("integer");
NodeType const test::FLOAT("float");
NodeType const test::BOOLEAN("boolean");

NodeType const test::REFERENCE("reference");
NodeType const test::CALL_BEGIN("call begin");
NodeType const test::CALL_END("call end");
NodeType const test::ARG_SEPARATOR("argument separator");
NodeType const test::FUNC_REFERENCE("func reference");
NodeType const test::FUNC_REF_NEXT_VAR("func reference next variable");

void InstanceTest::SetUp()
{
    clearErr();
}

void InstanceTest::TearDown()
{
    DataTree::verify();
}
