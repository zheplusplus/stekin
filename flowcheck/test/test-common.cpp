#include "test-common.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"
#include "../../test/phony-warnings.h"
#include "../../instance/node-base.h"
#include "../../util/string.h"

using namespace test;

util::sref<inst::scope> const test::nul_inst_scope(NULL);

data_tree& data_tree::operator()(misc::position const& pos, node_type const& type, std::string const& str)
{
    base_type::operator()(type, flowcheck_data(pos), str);
    return *this;
}

data_tree& data_tree::operator()(misc::position const& pos, node_type const& type)
{
    base_type::operator()(type, flowcheck_data(pos));
    return *this;
}

data_tree& data_tree::operator()(misc::position const& pos
                               , node_type const& type
                               , std::string const& str
                               , int func_arg_size
                               , bool func_hint_ret_void)
{
    base_type::operator()(type, flowcheck_data(pos, func_arg_size, func_hint_ret_void), str);
    return *this;
}

data_tree& data_tree::operator()(node_type const& type)
{
    base_type::operator()(type, flowcheck_data());
    return *this;
}

std::ostream& operator<<(std::ostream& os, flowcheck_data const& data)
{
    return -1 == data.func_arg_size
        ? (os << data.pos)
        : (os << data.pos << " arg size=" << data.func_arg_size
                          << " returning void=" << util::str(data.func_hint_ret_void));
}

node_type const test::BOOLEAN("boolean");
node_type const test::INTEGER("integer");
node_type const test::FLOATING("floating");
node_type const test::BINARY_OP("binary operation");
node_type const test::PRE_UNARY_OP("prefix unary operation");
node_type const test::CALL("call");
node_type const test::FUNC_REFERENCE("func reference");
node_type const test::REFERENCE("var reference");

node_type const test::VAR_DEF("var definition");
node_type const test::SCOPE_VAR_DEF("scope var definition");

node_type const test::STATEMENT("statement");
node_type const test::ARITHMETICS("arithmetics");
node_type const test::RETURN("return");
node_type const test::RETURN_NOTHING("return nothing");

node_type const test::FUNC_DECL("func decl");
node_type const test::PARAMETER("parameter");

node_type const test::SCOPE_BEGIN("scope begin");
node_type const test::SCOPE_END("scope end");

node_type const test::BRANCH("branch");

void flowcheck_test::SetUp()
{
    clear_err();
    clear_warn();
}

void flowcheck_test::TearDown()
{
    data_tree::verify();
}
