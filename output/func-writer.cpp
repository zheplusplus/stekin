#include <iostream>
#include <algorithm>

#include "func-writer.h"
#include "statement-writer.h"
#include "name-mangler.h"
#include "../util/string.h"

using namespace output;

static std::string const FUNC_DECL(
    "struct $FUNC_NAME {\n"
    "    char _stk_frame_space[$FUNC_FRAME_SIZE];\n"
    "    _stk_frame_bases<$FUNC_LEVEL> _stk_bases;\n"
    "\n"
    "    template <int _ExtLevel>\n"
    "    $FUNC_NAME(\n"
    "          _stk_frame_bases<_ExtLevel> const& ext_bases\n"
    "          $ARGS_DECL\n"
    "            )\n"
    "        : _stk_bases(ext_bases, _stk_frame_space)\n"
    "    {\n"
    "        $COPY_ARGS\n"
    "    }\n"
    "\n"
    "    $FUNC_NAME()\n"
    "        : _stk_bases(_stk_frame_space)\n"
    "    {}\n"
    "\n"
    "    $FUNC_RET_TYPE _stk_perform();\n"
    "};\n"
);

static std::string const FUNC_PERFORM_IMPL_BEGIN(
    "$FUNC_RET_TYPE $FUNC_NAME::_stk_perform()\n"
);

static std::string form_args_decl(std::list<stack_var_record> const& params)
{
    int i = 0;
    std::string result;
    std::for_each(params.begin()
                , params.end()
                , [&](stack_var_record const& record)
                  {
                      result += ", " + record.type + " _stk_arg_" + util::str(i++);
                  });
    return result;
}

static std::string form_copy_args(std::list<stack_var_record> const& params)
{
    int i = 0;
    std::string result;
    std::for_each(params.begin()
                , params.end()
                , [&](stack_var_record const& record)
                  {
                      result += "_stk_bases.push("
                              + util::str(record.offset)
                              + ", _stk_arg_"
                              + util::str(i++)
                              + ");";
                  });
    return result;
}

void output::write_func_decl(std::string const& ret_type_name
                           , util::id func_addr
                           , std::list<stack_var_record> const& params
                           , int func_level
                           , int stack_size_used)
{
    std::vector<std::string> typenames;
    std::for_each(params.begin()
                , params.end()
                , [&](stack_var_record const& record)
                  {
                      typenames.push_back(record.type);
                  });
    std::cout <<
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
            FUNC_DECL
                , "$FUNC_RET_TYPE", ret_type_name)
                , "$FUNC_NAME", form_func_name(func_addr))
                , "$ARGS_DECL", form_args_decl(params))
                , "$COPY_ARGS", form_copy_args(params))
                , "$FUNC_LEVEL", util::str(func_level))
                , "$FUNC_FRAME_SIZE", util::str(stack_size_used))
    ;
}

void output::write_func_perform_impl(std::string const& ret_type_name, util::id func_addr)
{
    std::cout <<
        util::replace_all(
        util::replace_all(
            FUNC_PERFORM_IMPL_BEGIN
                , "$FUNC_RET_TYPE", ret_type_name)
                , "$FUNC_NAME", form_func_name(func_addr))
    ;
}

void output::writeCallBegin(util::id func_addr)
{
    std::cout << "(" << form_func_name(func_addr) << "(_stk_bases";
}

void output::write_arg_seperator()
{
    std::cout << ", ";
}

void output::writeCallEnd()
{
    std::cout << ")._stk_perform())";
}

static std::string const MAIN_BEGIN(
    "int main()\n"
    "{\n"
);

static std::string const MAIN_END(
    "    return 0;\n"
    "}\n"
);

void output::write_main_begin()
{
    std::cout << MAIN_BEGIN;
}

void output::write_main_end()
{
    std::cout << MAIN_END << std::endl;
}

void output::stk_main_func(util::id func_addr)
{
    std::cout << "    " << form_func_name(func_addr) << "()._stk_perform();" << std::endl;
}

void output::constructFuncReference(std::string const& type_exported_name)
{
    std::cout << type_exported_name << "()";
}

void output::funcReferenceNextVariable(int offset, stack_var_record const& init)
{
    std::cout << (".push(" + util::str(offset) + ", ");
    ref_level(init.offset, init.level, init.type);
    std::cout << ')';
}
