#include "test-common.h"

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

DataTree& DataTree::operator()(NodeType const& type, std::string const& name, int offset)
{
    BaseType::operator()(type, InstanceData(offset), name);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, int level, std::string const& name)
{
    BaseType::operator()(type, InstanceData(level, -1), name);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type, int offset)
{
    BaseType::operator()(type, InstanceData(offset));
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

std::string const test::PARAM("parameter: ");
std::string const test::RES_PARAM("resource parameter: ");

NodeType const test::FUNC_DECL_BEGIN("func declaration begin");
NodeType const test::FUNC_DECL_END("func declaration end");
NodeType const test::FUNC_DEF("func definition");
NodeType const test::PARAMETER("parameter");

NodeType const test::BLOCK_BEGIN("block begin");
NodeType const test::BLOCK_END("block end");
NodeType const test::END_OF_STATEMENT("end of statement");

NodeType const test::INTEGER("integer");
NodeType const test::FLOAT("float");
NodeType const test::BOOLEAN("boolean");
NodeType const test::STRING("string");
NodeType const test::EMPTY_LIST("empty list");
NodeType const test::LIST_BEGIN("list begin");
NodeType const test::LIST_NEXT_MEMBER("list next member");
NodeType const test::LIST_END("list end");

NodeType const test::LIST_APPEND_BEGIN("list append begin");
NodeType const test::LIST_APPEND_END("list append end");

NodeType const test::PIPE_MAP_BEGIN("pipe map begin");
NodeType const test::PIPE_MAP_END("pipe map end");
NodeType const test::PIPE_FILTER_BEGIN("pipe filter begin");
NodeType const test::PIPE_FILTER_END("pipe filter end");
NodeType const test::PIPE_BEGIN("pipe begin");
NodeType const test::PIPE_END("pipe end");
NodeType const test::PIPE_ELEMENT("pipe element");
NodeType const test::PIPE_INDEX("pipe index");

NodeType const test::INITIALIZE_THIS_LEVEL("initialize this level");
NodeType const test::REFERENCE("reference");
NodeType const test::CALL_BEGIN("call begin");
NodeType const test::CALL_END("call end");
NodeType const test::ARG_SEPARATOR("argument separator");
NodeType const test::FUNC_REFERENCE("func reference");
NodeType const test::FUNC_REF_NEXT_VAR("func reference next variable");
NodeType const test::FUNC_RES_ENTRY("func reference resource entry");
NodeType const test::ADD_RES_ENTRY("add reference resource entry");

NodeType const test::OPERATOR("operator");
NodeType const test::EXPRESSION_BEGIN("expression begin");
NodeType const test::EXPRESSION_END("expression end");

NodeType const test::WRITER_BEGIN("writer begin");
NodeType const test::WRITER_END("writer end");
NodeType const test::MEMBER_CALL_BEGIN("member call begin");
NodeType const test::MEMBER_CALL_END("member call end");

NodeType const test::BRANCH_IF("branch if");
NodeType const test::BRANCH_ELSE("branch else");

NodeType const test::RETURN("return");
NodeType const test::RETURN_NOTHING("return nothing");

void InstanceTest::TearDown()
{
    DataTree::verify();
}
