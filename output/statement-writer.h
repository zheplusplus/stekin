#ifndef __STEKIN_OUTPUT_STATEMENT_WRITER_H__
#define __STEKIN_OUTPUT_STATEMENT_WRITER_H__

#include <string>

#include "../misc/platform.h"

namespace output {

    void kwReturn();
    void returnNothing();
    void refThisLevel(int offset, std::string const& type_exported_name);
    void refLevel(int offset, int level, std::string const& type_exported_name);
    void signAssign();

    void writeInt(platform::int_type i);
    void writeFloat(platform::float_type d);
    void writeBool(bool b);
    void writeOperator(std::string const& op_img);

    void beginExpr();
    void endExpr();

    void branchIf();
    void branchElse();

    void blockBegin();
    void blockEnd();
    void endOfStatement();

    void beginWriteStmt();
    void endWriteStmt();

}

#endif /* __STEKIN_OUTPUT_STATEMENT_WRITER_H__ */
