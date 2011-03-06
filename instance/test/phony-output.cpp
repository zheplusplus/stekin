#include "../../output/func-writer.h"
#include "../../output/statement-writer.h"

using namespace output;

void output::write_func_decl(std::string const&, util::id, std::list<stack_var_record> const&, int, int) {}
void output::write_func_perform_impl(std::string const&, util::id) {}
void output::write_call_begin(util::id) {}
void output::write_arg_seperator() {}
void output::write_call_end() {}
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
void output::branch_if() {}
void output::branch_else() {}
void output::block_begin() {}
void output::block_end() {}
void output::end_of_statement() {}
