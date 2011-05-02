#include "test-common.h"
#include "../../instance/expr-nodes.h"
#include "../../instance/function.h"
#include "../../util/string.h"

using namespace test;
using namespace inst;

util::sref<Type const> const Type::BIT_VOID(util::mkref(test::PROTO_TEST_VOID));

static PhonyFunc func;
static bool test_func_inst_resolved = false;
static int path_count = 0;

bool Type::operator!=(Type const& rhs) const
{
    return !operator==(rhs);
}

bool Type::eqAsBuiltIn(Type const&) const
{
    return false;
}

bool Type::eqAsFuncReference(util::sref<proto::Function>
                           , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool Type::ltAsBuiltIn(Type const&) const
{
    return false;
}

bool Type::ltAsFuncReference(util::sref<proto::Function>
                           , std::map<std::string, Variable const> const&) const
{
    return false;
}

std::string BuiltInPrimitive::exportedName() const
{
    return tname;
}

std::string BuiltInPrimitive::name() const
{
    return tname;
}

bool BuiltInPrimitive::operator==(Type const& rhs) const
{
    return this == &rhs;
}

bool BuiltInPrimitive::operator<(Type const& rhs) const
{
    return this < &rhs;
}

bool BuiltInPrimitive::eqAsBuiltIn(Type const& lhs) const
{
    return &lhs == this;
}

bool BuiltInPrimitive::ltAsBuiltIn(Type const& lhs) const
{
    return &lhs < this;
}

bool BuiltInPrimitive::ltAsFuncReference(util::sref<proto::Function>
                                       , std::map<std::string, Variable const> const&) const
{
    return false;
}

std::string FuncReferenceType::exportedName() const
{
    return "";
}

std::string FuncReferenceType::name() const
{
    return "";
}

bool FuncReferenceType::operator==(Type const& rhs) const
{
    return this == &rhs;
}

bool FuncReferenceType::operator<(Type const& rhs) const
{
    return this < &rhs;
}

bool FuncReferenceType::eqAsFuncReference(util::sref<proto::Function>
                                        , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool FuncReferenceType::ltAsFuncReference(util::sref<proto::Function>
                                        , std::map<std::string, Variable const> const&) const
{
    return false;
}

bool FuncReferenceType::ltAsBuiltIn(Type const&) const
{
    return false;
}

void Type::checkCondType(misc::position const&) const {}
void BuiltInPrimitive::checkCondType(misc::position const&) const {}

util::sptr<inst::Expression const> FuncReferenceType::call(
                                          misc::position const&
                                        , int
                                        , int
                                        , std::vector<util::sref<inst::Type const>> const&
                                        , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

std::map<std::string, Variable const> FuncReferenceType::_encloseReference(
                                          misc::position const&
                                        , int
                                        , std::map<std::string, Variable const> const& cr)
{
    return cr;
}

int FuncReferenceType::_calcSize(std::map<std::string, Variable const> const&)
{
    return 0;
}

util::sptr<inst::Expression const> BuiltInPrimitive::call(
                                          misc::position const&
                                        , int
                                        , int
                                        , std::vector<util::sref<inst::Type const>> const&
                                        , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

util::sptr<inst::Expression const> Variable::call(misc::position const&
                                                , std::vector<util::sref<inst::Type const>> const&
                                                , std::vector<util::sptr<Expression const>>) const
{
    return std::move(util::sptr<inst::Expression const>(NULL));
}

bool Variable::operator<(Variable const& rhs) const
{
    return this < &rhs;
}

bool Variable::operator==(Variable const& rhs) const
{
    return this == &rhs;
}

bool Variable::operator!=(Variable const& rhs) const
{
    return !operator==(rhs);
}

void Function::setReturnType(misc::position const& pos, util::sref<Type const> t)
{
    test_func_inst_resolved = true;
    DataTree::actualOne()(pos, t == Type::BIT_VOID ? SET_RETURN_TYPE_VOID : SET_RETURN_TYPE);
}

util::sref<Type const> Function::getReturnType() const
{
    return util::mkref(PROTO_TEST_TYPE);
}

bool Function::isReturnTypeResolved() const
{
    DataTree::actualOne()(QUERY_RETURN_TYPE_RESOLVE_STATUS);
    return test_func_inst_resolved ;
}

void Function::addPath(util::sref<MediateBase>)
{
    DataTree::actualOne()(ADD_PATH);
    ++path_count;
}

void Function::instNextPath()
{
    DataTree::actualOne()(NEXT_PATH);
    --path_count;
}

bool Function::hasMorePath() const
{
    return path_count > 0;
}

Variable Function::defVar(misc::position const& pos, util::sref<Type const> t, std::string const&)
{
    return Variable(pos, t, 0, 0);
}

Variable Function::queryVar(misc::position const& pos, std::string const& name) const
{
    DataTree::actualOne()(pos, QUERY_VAR, name);
    return Variable(misc::position(0), util::mkref(test::PROTO_TEST_TYPE), 0, 0);
}

Operation const* Scope::queryBinary(misc::position const& pos
                                  , std::string const& op
                                  , util::sref<Type const>
                                  , util::sref<Type const>) const
{
    DataTree::actualOne()(pos, QUERY_BINARY_OP, op);
    return NULL;
}

Operation const* Scope::queryPreUnary(misc::position const& pos
                                    , std::string const& op
                                    , util::sref<Type const>) const
{
    DataTree::actualOne()(pos, QUERY_PRE_UNARY_OP, op);
    return NULL;
}

void Scope::addStmt(util::sptr<Statement const>)
{
    DataTree::actualOne()(ADD_STMT_TO_SCOPE);
}

int Function::level() const
{
    return 0;
}

SymbolTable::SymbolTable(int
                       , std::list<ArgNameTypeRec> const&
                       , std::map<std::string, Variable const> const&)
    : level(0)
    , _ss_used(0)
{}

util::sref<Function> Function::createInstance(int
                                            , std::list<ArgNameTypeRec> const&
                                            , std::map<std::string, Variable const> const&
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

util::sref<Type const> IntLiteral::typeof() const
{
    DataTree::actualOne()(INTEGER, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> FloatLiteral::typeof() const
{
    DataTree::actualOne()(FLOATING, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> BoolLiteral::typeof() const
{
    DataTree::actualOne()(BOOLEAN, util::str(value));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> Reference::typeof() const
{
    DataTree::actualOne()(REFERENCE);
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> Call::typeof() const
{
    DataTree::actualOne()(CALL, util::str(int(args.size())));
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> FuncReference::typeof() const
{
    DataTree::actualOne()(FUNC_REFERENCE);
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> BinaryOp::typeof() const
{
    DataTree::actualOne()(BINARY_OP);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> PreUnaryOp::typeof() const
{
    DataTree::actualOne()(PRE_UNARY_OP);
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> Conjunction::typeof() const
{
    DataTree::actualOne()(CONJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> Disjunction::typeof() const
{
    DataTree::actualOne()(DISJUNCTION);
    lhs->typeof();
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

util::sref<Type const> Negation::typeof() const
{
    DataTree::actualOne()(NEGATION);
    rhs->typeof();
    return util::mkref(PROTO_TEST_TYPE);
}

Conjunction::Conjunction(misc::position const&
                       , util::sptr<Expression const> l
                       , util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{}

Disjunction::Disjunction(misc::position const&
                       , util::sptr<Expression const> l
                       , util::sptr<Expression const> r)
    : lhs(std::move(l))
    , rhs(std::move(r))
{}

Negation::Negation(misc::position const&, util::sptr<Expression const> r)
    : rhs(std::move(r))
{}

Branch::Branch(misc::position const&
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
void Reference::write() const {}
void Call::write() const {}
void FuncReference::write() const {}
void BinaryOp::write() const {}
void PreUnaryOp::write() const {}
void Conjunction::write() const {}
void Disjunction::write() const {}
void Negation::write() const {}
void Arithmetics::write() const {}
void Branch::write() const {}
void Initialization::write() const {}
void Return::write() const {}
void ReturnNothing::write() const {}
void Block::write() const {}

#include "../../output/statement-writer.h"
void output::beginWriteStmt() {}
void output::endWriteStmt() {}
