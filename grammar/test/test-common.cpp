#include "test-common.h"
#include "../../flowcheck/filter.h"
#include "../../flowcheck/node-base.h"
#include "../../flowcheck/function.h"
#include "../../proto/node-base.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

util::sref<proto::scope> const test::nulscope(NULL);

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type, std::string const& str)
{
    BaseType::operator()(type, grammar_data(pos), str);
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type)
{
    BaseType::operator()(type, grammar_data(pos));
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos
                               , NodeType const& type
                               , std::string const& str
                               , int func_arg_size)
{
    BaseType::operator()(type, grammar_data(pos, func_arg_size), str);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type)
{
    BaseType::operator()(type, grammar_data());
    return *this;
}

std::ostream& operator<<(std::ostream& os, grammar_data const& data)
{
    return -1 == data.func_arg_size ? (os << data.pos) : (os << data.pos << " arg size=" << data.func_arg_size);
}

std::string const test::VAR_DEF_FILTERED(" var def filtered");
std::string const test::FUNC_DEF_FILTERED(" func def filtered");

NodeType const test::BOOLEAN("boolean");
NodeType const test::INTEGER("integer");
NodeType const test::FLOATING("floating");
NodeType const test::REFERENCE("reference");

NodeType const test::BINARY_OP("binary operation");
NodeType const test::PRE_UNARY_OP("prefix unary operation");

NodeType const test::CALL("call");
NodeType const test::FUNC_REFERENCE("Function reference");

NodeType const test::VAR_DEF("var def");
NodeType const test::ARITHMETICS("arithmetics");
NodeType const test::RETURN("return");
NodeType const test::RETURN_NOTHING("return nothing");

NodeType const test::FUNC_DEF("func def");
NodeType const test::PARAMETER("parameter");

NodeType const test::BRANCH("branch");
NodeType const test::BRANCH_CONSQ_ONLY("branch consequence only");
NodeType const test::BRANCH_ALTER_ONLY("branch alternative only");
NodeType const test::CONSEQUENCE("consequence");
NodeType const test::ALTERNATIVE("alternative");

NodeType const test::BLOCK_BEGIN("Block begin");
NodeType const test::BLOCK_END("Block end");

void grammar_test::SetUp()
{
    clear_err();
}

void grammar_test::TearDown()
{
    DataTree::verify();
}
