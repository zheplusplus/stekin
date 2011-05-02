#include "test-common.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"
#include "../../test/phony-warnings.h"
#include "../../proto/function.h"
#include "../../instance/node-base.h"
#include "../../util/string.h"

using namespace test;

util::sref<inst::Scope> const test::nul_inst_scope(NULL);

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str)
{
    BaseType::operator()(type, FlowcheckData(pos), str);
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos, NodeType const& type)
{
    BaseType::operator()(type, FlowcheckData(pos));
    return *this;
}

DataTree& DataTree::operator()(misc::position const& pos
                             , NodeType const& type
                             , std::string const& str
                             , int func_arg_size
                             , bool func_hint_ret_void)
{
    BaseType::operator()(type, FlowcheckData(pos, func_arg_size, func_hint_ret_void), str);
    return *this;
}

DataTree& DataTree::operator()(NodeType const& type)
{
    BaseType::operator()(type, FlowcheckData());
    return *this;
}

std::ostream& operator<<(std::ostream& os, FlowcheckData const& data)
{
    return -1 == data.func_arg_size
        ? (os << data.pos)
        : (os << data.pos << " arg size=" << data.func_arg_size
                          << " returning void=" << util::str(data.func_hint_ret_void));
}

NodeType const test::BOOLEAN("boolean");
NodeType const test::INTEGER("integer");
NodeType const test::FLOATING("floating");
NodeType const test::BINARY_OP("binary operation");
NodeType const test::PRE_UNARY_OP("prefix unary operation");
NodeType const test::CALL("call");
NodeType const test::FUNC_REFERENCE("func reference");
NodeType const test::FUNCTOR("functor");
NodeType const test::REFERENCE("var reference");

NodeType const test::VAR_DEF("var definition");

NodeType const test::STATEMENT("statement");
NodeType const test::ARITHMETICS("arithmetics");
NodeType const test::RETURN("return");
NodeType const test::RETURN_NOTHING("return nothing");

NodeType const test::FUNC_DECL("func decl");
NodeType const test::PARAMETER("parameter");

NodeType const test::SCOPE_BEGIN("scope begin");
NodeType const test::SCOPE_END("scope end");

NodeType const test::BRANCH("branch");

void FlowcheckTest::SetUp()
{
    clearErr();
    clearWarn();
}

void FlowcheckTest::TearDown()
{
    DataTree::verify();
}
