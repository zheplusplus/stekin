#ifndef __STEKIN_OUTPUT_FUNCTION_WRITER_H__
#define __STEKIN_OUTPUT_FUNCTION_WRITER_H__

#include <list>
#include <string>

#include "../util/pointer.h"

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
                     , util::id func_addr
                     , std::list<StackVarRec> const& params
                     , int func_level
                     , int stack_size_used);

    void writeFuncImpl(std::string const& ret_type_name, util::id func_addr);

    void writeCallBegin(util::id func_addr);
    void writeArgSeparator();
    void writeCallEnd();

    void writeMainBegin();
    void writeMainEnd();
    void stknMainFunc(util::id func_addr);

    void writeFuncReference(int size);
    void funcReferenceNextVariable(int offset, StackVarRec const& init);

}

#endif /* __STEKIN_OUTPUT_FUNCTION_WRITER_H__ */
