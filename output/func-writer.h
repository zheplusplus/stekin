#ifndef __STEKIN_OUTPUT_FUNCTION_WRITER_H__
#define __STEKIN_OUTPUT_FUNCTION_WRITER_H__

#include <list>
#include <string>

#include "../util/pointer.h"

namespace output {

    struct stack_var_record {
        std::string const type;
        int const offset;
        int const level;

        stack_var_record(std::string const& t, int o, int l)
            : type(t)
            , offset(o)
            , level(l)
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

    void construct_func_reference(std::string const& type_exported_name);
    void func_reference_next_variable(int offset, stack_var_record const& init);

}

#endif /* __STEKIN_OUTPUT_FUNCTION_WRITER_H__ */
