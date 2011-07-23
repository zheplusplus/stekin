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
    return os;
}

NodeType const test::FUNC_DECL_BEGIN("func declaration begin");
NodeType const test::FUNC_DECL_END("func declaration end");
NodeType const test::FUNC_DEF("func definition");
NodeType const test::PARAMETER("parameter");

NodeType const test::BLOCK_BEGIN("block begin");
NodeType const test::BLOCK_END("block end");

NodeType const test::INTEGER("integer");

void InstanceTest::SetUp()
{
    clearErr();
}

void InstanceTest::TearDown()
{
    DataTree::verify();
}
