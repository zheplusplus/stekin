#ifndef __STEKIN_OUTPUT_FUNCTION_WRITER_H__
#define __STEKIN_OUTPUT_FUNCTION_WRITER_H__

#include <list>
#include <string>

#include "../util/sn.h"

namespace output {

    struct StackVarRec {
        std::string const type;
        int const offset;
        int const level;

        StackVarRec(std::string const& t, int o, int l)
            : type(t)
            , offset(o)
            , level(l)
        {}
    };

    void writeFuncDecl(std::string const& ret_type_name
                     , util::serial_num func_sn
                     , std::list<StackVarRec> const& params
                     , int func_level
                     , int stack_size_used);

    void writeFuncImpl(std::string const& ret_type_name, util::serial_num func_sn);

    void writeCallBegin(util::serial_num func_sn);
    void writeArgSeparator();
    void writeCallEnd();

    void writeMainBegin();
    void writeMainEnd();
    void stknMainFunc(util::serial_num func_sn);

    void writeFuncReference(int size);
    void funcReferenceNextVariable(int offset, StackVarRec const& init);

}

#endif /* __STEKIN_OUTPUT_FUNCTION_WRITER_H__ */
