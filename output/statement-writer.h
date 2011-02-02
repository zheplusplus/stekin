#ifndef __STACKENING_OUTPUT_STATEMENT_WRITER_H__
#define __STACKENING_OUTPUT_STATEMENT_WRITER_H__

#include <string>

namespace output {

    void return_kw();
    void ref_level(int offset, int level, std::string const& type);
    void assign_sign();

    void write_int(int i);
    void write_float(double d);
    void write_bool(bool b);
    void write_operator(std::string const& op_img);

    void begin_expr();
    void end_expr();

    void branch_if();
    void branch_else();

    void loop_while();

    void block_begin();
    void block_end();
    void end_of_statement();

    void begin_write_stmt();
    void end_write_stmt();

}

#endif /* __STACKENING_OUTPUT_STATEMENT_WRITER_H__ */
