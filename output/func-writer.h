#ifndef __STACKENING_OUTPUT_FUNCTION_WRITER_H__
#define __STACKENING_OUTPUT_FUNCTION_WRITER_H__

#include <list>
#include <string>

#include "../util/pointer.h"

namespace output {

    struct stack_var_record {
        std::string const type;
        int const offset;
        int const size;

        stack_var_record(std::string const& t, int o, int s)
            : type(t)
            , offset(o)
            , size(s)
        {}
    };

    void write_func_decl(std::string const& ret_type_name
                       , util::id func_addr
                       , std::list<stack_var_record> const& params
                       , int func_level
                       , int stack_size_used);

    void write_func_perform_impl(std::string const& ret_type_name, util::id func_addr);

    void write_call_begin(util::id func_addr);
    void write_arg_seperator();
    void write_call_end();

    void write_main_begin();
    void write_main_end();
    void stk_main_func(util::id func_addr);

}

#endif /* __STACKENING_OUTPUT_FUNCTION_WRITER_H__ */
