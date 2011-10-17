#ifndef __STEKIN_OUTPUT_FUNCTION_WRITER_H__
#define __STEKIN_OUTPUT_FUNCTION_WRITER_H__

#include <list>
#include <vector>
#include <string>

#include <util/sn.h>
#include <util/pointer.h>

namespace output {

    struct StackVarRec {
        virtual ~StackVarRec() {}
        virtual std::string resEntry() const = 0;

        StackVarRec(std::string const& t, int o, int l)
            : type(t)
            , offset(o)
            , level(l)
        {}

        std::string const type;
        int const offset;
        int const level;
    };

    struct Parameter
        : public StackVarRec
    {
        Parameter(std::string const& type, int offset, int level)
            : StackVarRec(type, offset, level)
        {}

        std::string resEntry() const;
    };

    struct ResourceParam
        : public StackVarRec
    {
        ResourceParam(std::string const& type, int offset, int level)
            : StackVarRec(type, offset, level)
        {}

        std::string resEntry() const;
    };

    void writeFuncDecl(std::string const& ret_type_name
                     , util::serial_num func_sn
                     , std::vector<util::sptr<StackVarRec const>> const& params
                     , int func_level
                     , int stack_size_used
                     , int res_entry_size);
    void writeFuncImpl(std::string const& ret_type_name, util::serial_num func_sn);

    void writeCallBegin(util::serial_num func_sn);
    void writeCallEnd();
    void writeArgSeparator();

    void writeMainBegin();
    void writeMainEnd();
    void stknMainFunc(util::serial_num func_sn);

    void writeFuncReference(int size);
    void funcReferenceNextVariable(int offset, util::sptr<StackVarRec const> init);

    void pipeMapBegin(util::id pipe_id
                    , int level
                    , std::string const& src_member_type
                    , std::string const& dst_member_type);
    void pipeMapEnd();
    void pipeFilterBegin(util::id pipe_id, int level, std::string const& member_type);
    void pipeFilterEnd();

    void pipeBegin(util::id pipe_id);
    void pipeEnd();

    void pipeElement();
    void pipeIndex();

}

#endif /* __STEKIN_OUTPUT_FUNCTION_WRITER_H__ */
