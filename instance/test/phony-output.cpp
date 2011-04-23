#include "../../output/func-writer.h"
#include "../../output/statement-writer.h"
#include "../../output/name-mangler.h"
#include "../../proto/function.h"
#include "../../proto/node-base.h"

using namespace output;

void output::write_func_decl(std::string const&, util::id, std::list<stack_var_record> const&, int, int) {}
void output::write_func_perform_impl(std::string const&, util::id) {}
void output::writeCallBegin(util::id) {}
void output::write_arg_seperator() {}
void output::writeCallEnd() {}
void output::return_kw() {}
void output::return_nothing() {}
void output::ref_this_level(int, std::string const&) {}
void output::ref_level(int, int, std::string const&) {}
void output::assign_sign() {}
void output::write_int(int) {}
void output::write_float(double) {}
void output::write_bool(bool) {}
void output::write_operator(std::string const&) {}
void output::begin_expr() {}
void output::end_expr() {}
void output::branchIf() {}
void output::branchElse() {}
void output::block_begin() {}
void output::block_end() {}
void output::endOfStatement() {}
void output::constructFuncReference(std::string const&) {}
void output::funcReferenceNextVariable(int, stack_var_record const&) {}

std::string output::form_FuncReferenceType(int)
{
    return "";
}

std::string output::form_type(std::string const&)
{
    return "";
}

util::sref<inst::Function> proto::Function::inst(int
                                               , std::map<std::string, inst::Variable const> const&
                                               , std::vector<util::sref<inst::Type const>> const&)
{
    return util::sref<inst::Function>(NULL);
}
