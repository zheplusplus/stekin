#include "../../output/func-writer.h"
#include "../../output/statement-writer.h"
#include "../../output/name-mangler.h"
#include "../../proto/function.h"
#include "../../proto/node-base.h"

using namespace output;

void output::writeFuncDecl(std::string const&, util::id, std::list<StackVarRec> const&, int, int) {}
void output::writeFuncImpl(std::string const&, util::id) {}
void output::writeCallBegin(util::id) {}
void output::writeArgSeparator() {}
void output::writeCallEnd() {}
void output::kwReturn() {}
void output::returnNothing() {}
void output::refThisLevel(int, std::string const&) {}
void output::refLevel(int, int, std::string const&) {}
void output::signAssign() {}
void output::writeInt(int) {}
void output::writeFloat(double) {}
void output::writeBool(bool) {}
void output::writeOperator(std::string const&) {}
void output::beginExpr() {}
void output::endExpr() {}
void output::branchIf() {}
void output::branchElse() {}
void output::blockBegin() {}
void output::blockEnd() {}
void output::endOfStatement() {}
void output::constructFuncReference(std::string const&) {}
void output::funcReferenceNextVariable(int, StackVarRec const&) {}
void output::beginWriteStmt() {}
void output::endWriteStmt() {}

std::string output::formFuncReferenceType(int)
{
    return "";
}

std::string output::formType(std::string const&)
{
    return "";
}

util::sref<inst::Function> proto::Function::inst(int
                                               , std::map<std::string, inst::Variable const> const&
                                               , std::vector<util::sref<inst::Type const>> const&)
{
    return util::sref<inst::Function>(NULL);
}
