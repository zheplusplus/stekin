#include "test-common.h"
#include "../../instance/expr-nodes.h"
#include "../../instance/function.h"
#include "../../util/string.h"

using namespace test;
using namespace inst;

util::sref<type const> const type::BIT_VOID(util::mkref(test::PROTO_TEST_VOID));
static phony_func func;
static bool test_func_inst_resolved = false;
static int path_count = 0;

bool type::operator!=(type const& rhs) const
{
    return !operator==(rhs);
}

bool type::eq_as_built_in(type const&) const
{
    return false;
}

bool type::eq_as_FuncReference(util::sref<proto::Function>, std::map<std::string, variable const> const&) const
{
    return false;
}

bool type::lt_as_built_in(type const&) const
{
    return false;
}

bool type::lt_as_FuncReference(util::sref<proto::Function>, std::map<std::string, variable const> const&) const
{
    return false;
}

std::string built_in_primitive::exported_name() const
{
    return tname;
}

std::string built_in_primitive::name() const
{
    return tname;
}

bool built_in_primitive::operator==(type const& rhs) const
{
    return this == &rhs;
}

bool built_in_primitive::operator<(type const& rhs) const
{
    return this < &rhs;
}

bool built_in_primitive::eq_as_built_in(type const& lhs) const
{
    return &lhs == this;
}

bool built_in_primitive::lt_as_built_in(type const& lhs) const
{
    return &lhs < this;
}

bool built_in_primitive::lt_as_FuncReference(util::sref<proto::Function>
                                            , std::map<std::string, variable const> const&) const
{
    return false;
}

std::string FuncReferenceType::exported_name() const
{
    return "";
}

std::string FuncReferenceType::name() const
{
    return "";
}

bool FuncReferenceType::operator==(type const& rhs) const
{
    return this == &rhs;
}

bool FuncReferenceType::operator<(type const& rhs) const
{
    return this < &rhs;
}

bool FuncReferenceType::eq_as_FuncReference(util::sref<proto::Function>
                                             , std::map<std::string, variable const> const&) const
{
    return false;
}

bool FuncReferenceType::lt_as_FuncReference(util::sref<proto::Function>
                                             , std::map<std::string, variable const> const&) const
{
    return false;
}

bool FuncReferenceType::lt_as_built_in(type const&) const
{
    return false;
}

void type::check_condition_type(misc::position const&) const {}
void built_in_primitive::check_condition_type(misc::position const&) const {}

util::sptr<inst::Expression const> FuncReferenceType::call_func(
                  misc::position const&
                , int
                , int
                , std::vector<util::sref<inst::type const>> const&
                , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

std::map<std::string, variable const> FuncReferenceType::_enclose_reference(
                                            misc::position const&
                                          , int
                                          , std::map<std::string, variable const> const& cr)
{
    return cr;
}

int FuncReferenceType::_calc_size(std::map<std::string, variable const> const&)
{
    return 0;
}

util::sptr<inst::Expression const> built_in_primitive::call_func(
                  misc::position const&
                , int
                , int
                , std::vector<util::sref<inst::type const>> const&
                , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

util::sptr<inst::Expression const> variable::call_func(misc::position const&
                                                    , std::vector<util::sref<inst::type const>> const&
                                                    , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

bool variable::operator<(variable const& rhs) const
{
    return this < &rhs;
}

bool variable::operator==(variable const& rhs) const
{
    return this == &rhs;
}

bool variable::operator!=(variable const& rhs) const
{
    return !operator==(rhs);
}

void Function::set_return_type(misc::position const& pos, util::sref<type const> t)
{
    test_func_inst_resolved = true;
    DataTree::actualOne()(pos, t == type::BIT_VOID ? SET_RETURN_TYPE_VOID : SET_RETURN_TYPE);
}

util::sref<type const> Function::get_return_type() const
{
    return util::mkref(PROTO_TEST_TYPE);
}

bool Function::is_return_type_resolved() const
{
    DataTree::actualOne()(QUERY_RETURN_TYPE_RESOLVE_STATUS);
    return test_func_inst_resolved ;
}

void Function::add_path(util::sref<mediate_base>)
{
    DataTree::actualOne()(ADD_PATH);
    ++path_count;
}

void Function::inst_next_path()
{
    DataTree::actualOne()(NEXT_PATH);
    --path_count;
}

bool Function::has_more_path() const
{
    return path_count > 0;
}

variable Function::defVar(misc::position const& pos, util::sref<type const> vtype, std::string const&)
{
    return variable(pos, vtype, 0, 0);
}

variable Function::query_var(misc::position const& pos, std::string const& name) const
{
    DataTree::actualOne()(pos, QUERY_VAR, name);
    return variable(misc::position(0), util::mkref(test::PROTO_TEST_TYPE), 0, 0);
}

operation const* scope::query_binary(misc::position const& pos
                                   , std::string const& op
                                   , util::sref<type const>
                                   , util::sref<type const>) const
{
    DataTree::actualOne()(pos, QUERY_BINARY_OP, op);
    return NULL;
}

operation const* scope::query_pre_unary(misc::position const& pos, std::string const& op, util::sref<type const>) const
{
    DataTree::actualOne()(pos, QUERY_PRE_UNARY_OP, op);
    return NULL;
}

void scope::addStmt(util::sptr<Statement const>)
{
    DataTree::actualOne()(ADD_STMT_TO_SCOPE);
}

int Function::level() const
{
    return 0;
}

symbol_table::symbol_table(int
                         , std::list<arg_name_type_pair> const&
                         , std::map<std::string, variable const> const&)
    : level(0)
    , _ss_used(0)
{}

util::sref<Function> Function::create_instance(int
                                             , std::list<arg_name_type_pair> const&
                                             , std::map<std::string, variable const> const&
                                             , bool has_void_returns)
{
    test_func_inst_resolved = false;
    path_count = 0;
    if (has_void_returns) {
        test_func_inst_resolved = true;
        DataTree::actualOne()(INIT_AS_VOID_RET);
    }
    return util::mkref(func);
}

void Block::addStmt(util::sptr<Statement const>)
{
    DataTree::actualOne()(ADD_STMT_TO_BLOCK);
}

util::sref<type const> IntLiteral::typeof() const
{
    DataTree::actualOne()(INTEGER, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> FloatLiteral::typeof() const
{
    DataTree::actualOne()(FLOATING, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> BoolLiteral::typeof() const
{
    DataTree::actualOne()(BOOLEAN, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> reference::typeof() const
{
    DataTree::actualOne()(REFERENCE);
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> call::typeof() const
{
    DataTree::actualOne()(CALL, util::str(int(args.size())));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> FuncReference::typeof() const
{
    DataTree::actualOne()(FUNC_REFERENCE);
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> BinaryOp::typeof() const
{
    DataTree::actualOne()(BINARY_OP);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> PreUnaryOp::typeof() const
{
    DataTree::actualOne()(PRE_UNARY_OP);
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> Conjunction::typeof() const
{
    DataTree::actualOne()(CONJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> Disjunction::typeof() const
{
    DataTree::actualOne()(DISJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<type const> Negation::typeof() const
{
    DataTree::actualOne()(NEGATION);
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

Conjunction::Conjunction(misc::position const&, util::sptr<Expression const> l, util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{}

Disjunction::Disjunction(misc::position const&, util::sptr<Expression const> l, util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{}

Negation::Negation(misc::position const&, util::sptr<Expression const> r)
    : rhs(std::move(r))
{}

branch::branch(misc::position const&
             , util::sptr<Expression const> p
             , util::sptr<Statement const> c
             , util::sptr<Statement const> a)
    : predicate(std::move(p))
    , consequence(std::move(c))
    , alternative(std::move(a))
{}

void IntLiteral::write() const {}
void FloatLiteral::write() const {}
void BoolLiteral::write() const {}
void reference::write() const {}
void call::write() const {}
void FuncReference::write() const {}
void BinaryOp::write() const {}
void PreUnaryOp::write() const {}
void Conjunction::write() const {}
void Disjunction::write() const {}
void Negation::write() const {}
void Arithmetics::write() const {}
void branch::write() const {}
void initialization::write() const {}
void Return::write() const {}
void ReturnNothing::write() const {}
void Block::write() const {}
