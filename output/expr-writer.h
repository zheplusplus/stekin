#ifndef __STEKIN_OUTPUT_EXPRESSION_WRITER_H__
#define __STEKIN_OUTPUT_EXPRESSION_WRITER_H__

#include <string>

#include <misc/platform.h>

namespace output {

    void writeInt(platform::int_type i);
    void writeFloat(platform::float_type d);
    void writeBool(bool b);

    void refLevel(int offset, int level, std::string const& type_exported_name);
    void writeOperator(std::string const& op_img);

    void emptyList();
    void listBegin(int size, std::string const& member_type_exported_name);
    void listNextMember();
    void listEnd();

    void memberCallBegin(std::string const& member_name);
    void memberCallEnd();

    void listAppendBegin();
    void listAppendEnd();

    void beginExpr();
    void endExpr();

}

#endif /* __STEKIN_OUTPUT_EXPRESSION_WRITER_H__ */
