#include <iostream>
#include <algorithm>

#include <util/string.h>

#include "func-writer.h"
#include "stmt-writer.h"
#include "expr-writer.h"
#include "name-mangler.h"

using namespace output;

std::string Parameter::resEntry() const
{
    return "";
}

std::string ResourceParam::resEntry() const
{
    return "_res_entries.add(" + util::str(offset) + ");";
}

static std::string const FUNC_DECL(
    "struct $FUNC_NAME {\n"
    "    char _stk_frame_space[$FUNC_FRAME_SIZE];\n"
    "    _stk_frame_bases<$FUNC_LEVEL> _stk_bases;\n"
    "    _stk_res_entries<$RES_ENTRIES_SIZE> _res_entries;\n"
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
    "\n"
    "    ~$FUNC_NAME()\n"
    "    {\n"
    "        _res_entries.dtor(_stk_frame_space);\n"
    "    }\n"
    "};\n"
);

static std::string const FUNC_PERFORM_IMPL_BEGIN("$FUNC_RET_TYPE $FUNC_NAME::_stk_perform()\n");

static std::string formArgsDecl(std::vector<util::sptr<StackVarRec const>> const& params)
{
    int i = 0;
    std::string result;
    std::for_each(params.begin()
                , params.end()
                , [&](util::sptr<StackVarRec const> const& record)
                  {
                      result += ", " + record->type + " _stk_arg_" + util::str(i++);
                  });
    return result;
}

static std::string formCopyArgs(std::vector<util::sptr<StackVarRec const>> const& params)
{
    int i = 0;
    std::string result;
    std::for_each(params.begin()
                , params.end()
                , [&](util::sptr<StackVarRec const> const& record)
                  {
                      result += "_stk_bases.push("
                              + util::str(record->offset)
                              + ", _stk_arg_"
                              + util::str(i++)
                              + ");"
                              + record->resEntry();
                  });
    return result;
}

void output::writeFuncDecl(std::string const& ret_type_name
                         , util::serial_num func_sn
                         , std::vector<util::sptr<StackVarRec const>> const& params
                         , int func_level
                         , int stack_size_used
                         , int res_entry_size)
{
    std::vector<std::string> typenames;
    std::for_each(params.begin()
                , params.end()
                , [&](util::sptr<StackVarRec const> const& record)
                  {
                      typenames.push_back(record->type);
                  });
    std::cout <<
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
            FUNC_DECL
                , "$FUNC_RET_TYPE", ret_type_name)
                , "$FUNC_NAME", formFuncName(func_sn))
                , "$RES_ENTRIES_SIZE", util::str(res_entry_size))
                , "$ARGS_DECL", formArgsDecl(params))
                , "$COPY_ARGS", formCopyArgs(params))
                , "$FUNC_LEVEL", util::str(func_level))
                , "$FUNC_FRAME_SIZE", util::str(stack_size_used))
    ;
}

void output::writeFuncImpl(std::string const& ret_type_name, util::serial_num func_sn)
{
    std::cout <<
        util::replace_all(
        util::replace_all(
            FUNC_PERFORM_IMPL_BEGIN
                , "$FUNC_RET_TYPE", ret_type_name)
                , "$FUNC_NAME", formFuncName(func_sn))
    ;
}

void output::writeCallBegin(util::serial_num func_sn)
{
    std::cout << "(" << formFuncName(func_sn) << "(_stk_bases";
}

void output::writeArgSeparator()
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

void output::writeMainBegin()
{
    std::cout << MAIN_BEGIN;
}

void output::writeMainEnd()
{
    std::cout << MAIN_END << std::endl;
}

void output::stknMainFunc(util::serial_num func_sn)
{
    std::cout << "    " << formFuncName(func_sn) << "()._stk_perform();" << std::endl;
}

void output::writeFuncReference(int size)
{
    std::cout << formFuncReferenceType(size) << "()";
}

void output::funcReferenceNextVariable(int offset, util::sptr<StackVarRec const> init)
{
    std::cout << (".push(" + util::str(offset) + ", ");
    refLevel(init->offset, init->level, init->type);
    std::cout << ')';
}

static std::string const PIPE_MAP_BEGIN(
"struct _stk_pipe_$PIPE_ID {\n"
"    _stk_frame_bases<$LEVEL> _stk_bases;\n"
"\n"
"    explicit _stk_pipe_$PIPE_ID(_stk_frame_bases<$LEVEL> const& cp_bases)\n"
"        : _stk_bases(cp_bases)\n"
"    {}\n"
"\n"
"    _stk_list<$DST_MEMBER_TYPE > _stk_perform(_stk_list<$SRC_MEMBER_TYPE > const& src)\n"
"    {\n"
"        _stk_list<$DST_MEMBER_TYPE > result(src._size);\n"
"        result._size = src._size;\n"
"        for (_stk_type_int _stk_index = 0; _stk_index < src._size; ++_stk_index) {\n"
"            result._members[_stk_index] = (\n"
);

static std::string const PIPE_MAP_END(
"                                           );\n"
"        }\n"
"        return result;\n"
"    }\n"
"};\n"
);

void output::pipeMapBegin(util::id pipe_id
                        , int level
                        , std::string const& src_member_type
                        , std::string const& dst_member_type)
{
    std::cout <<
        util::replace_all(
        util::replace_all(
        util::replace_all(
        util::replace_all(
            PIPE_MAP_BEGIN
                , "$PIPE_ID", pipe_id.str())
                , "$LEVEL", util::str(level))
                , "$SRC_MEMBER_TYPE", src_member_type)
                , "$DST_MEMBER_TYPE", dst_member_type)
    ;
}

void output::pipeMapEnd()
{
    std::cout << PIPE_MAP_END;
}

static std::string const PIPE_FILTER_BEGIN(
"struct _stk_pipe_$PIPE_ID {\n"
"    _stk_frame_bases<$LEVEL> _stk_bases;\n"
"\n"
"    explicit _stk_pipe_$PIPE_ID(_stk_frame_bases<$LEVEL> const& cp_bases)\n"
"        : _stk_bases(cp_bases)\n"
"    {}\n"
"\n"
"    _stk_list<$MEMBER_TYPE > _stk_perform(_stk_list<$MEMBER_TYPE > const& src)\n"
"    {\n"
"        _stk_list<$MEMBER_TYPE > result(src._size);\n"
"        _stk_type_int cursor = 0;\n"
"        for (_stk_type_int _stk_index = 0; _stk_index < src._size; ++_stk_index) {\n"
"            if (\n"
);

static std::string const PIPE_FILTER_END(
"                ) {\n"
"                result._members[cursor] = src._members[_stk_index];\n"
"                ++cursor;\n"
"            }\n"
"        }\n"
"        result._size = cursor;\n"
"        return result;\n"
"    }\n"
"};\n"
);

void output::pipeFilterBegin(util::id pipe_id, int level, std::string const& member_type)
{
    std::cout <<
        util::replace_all(
        util::replace_all(
        util::replace_all(
            PIPE_FILTER_BEGIN
                , "$PIPE_ID", pipe_id.str())
                , "$LEVEL", util::str(level))
                , "$MEMBER_TYPE", member_type)
    ;
}

void output::pipeFilterEnd()
{
    std::cout << PIPE_FILTER_END;
}

static std::string const PIPE_BEGIN("_stk_pipe_$PIPE_ID(_stk_bases)._stk_perform(");
static std::string const PIPE_END(")");

void output::pipeBegin(util::id pipe_id)
{
    std::cout << util::replace_all(PIPE_BEGIN, "$PIPE_ID", pipe_id.str());
}

void output::pipeEnd()
{
    std::cout << PIPE_END;
}

void output::pipeElement()
{
    std::cout << "src._members[_stk_index]";
}

void output::pipeIndex()
{
    std::cout << "_stk_index";
}
