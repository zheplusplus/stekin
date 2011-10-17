#include <output/func-writer.h>
#include <output/stmt-writer.h>
#include <output/expr-writer.h>
#include <output/built-in-writer.h>
#include <output/name-mangler.h>
#include <util/string.h>

#include "test-common.h"

using namespace test;
using namespace output;

void output::writeFuncDecl(std::string const& return_type_name
                         , util::serial_num
                         , std::vector<util::sptr<StackVarRec const>> const& var_recs
                         , int level
                         , int stack_size
                         , int res_entry_size)
{
    DataTree::actualOne()(FUNC_DECL_BEGIN, return_type_name, level, stack_size);
    DataTree::actualOne()(FUNC_RES_ENTRY, res_entry_size);
    std::for_each(var_recs.begin()
                , var_recs.end()
                , [&](util::sptr<StackVarRec const> const& var)
                  {
                      DataTree::actualOne()(PARAMETER, var->type, var->level, var->offset);
                  });
    DataTree::actualOne()(FUNC_DECL_END);
}

void output::writeFuncImpl(std::string const& return_type_name, util::serial_num)
{
    DataTree::actualOne()(FUNC_DEF, return_type_name);
}

void output::writeCallBegin(util::serial_num)
{
    DataTree::actualOne()(CALL_BEGIN);
}

void output::writeArgSeparator()
{
    DataTree::actualOne()(ARG_SEPARATOR);
}

void output::writeCallEnd()
{
    DataTree::actualOne()(CALL_END);
}

void output::writeFuncReference(int size)
{
    DataTree::actualOne()(FUNC_REFERENCE, util::str(size));
}

void output::funcReferenceNextVariable(int self_offset, util::sptr<StackVarRec const> rec)
{
    DataTree::actualOne()(FUNC_REF_NEXT_VAR, rec->type, rec->level, rec->offset, self_offset);
}

void output::pipeMapBegin(util::id pipe_id
                        , int level
                        , std::string const& src_member_type
                        , std::string const& dst_member_type)
{
    DataTree::actualOne()(PIPE_MAP_BEGIN, level, pipe_id.str());
    DataTree::actualOne()(PIPE_MAP_BEGIN, src_member_type);
    DataTree::actualOne()(PIPE_MAP_BEGIN, dst_member_type);
}

void output::pipeMapEnd()
{
    DataTree::actualOne()(PIPE_MAP_END);
}

void output::pipeFilterBegin(util::id pipe_id, int level, std::string const& member_type)
{
    DataTree::actualOne()(PIPE_FILTER_BEGIN, level, member_type);
}

void output::pipeFilterEnd()
{
    DataTree::actualOne()(PIPE_FILTER_END);
}

void output::pipeBegin(util::id pipe_id)
{
    DataTree::actualOne()(PIPE_BEGIN, pipe_id.str());
}

void output::pipeEnd()
{
    DataTree::actualOne()(PIPE_END);
}

void output::pipeElement()
{
    DataTree::actualOne()(PIPE_ELEMENT);
}

void output::pipeIndex()
{
    DataTree::actualOne()(PIPE_INDEX);
}

void output::kwReturn()
{
    DataTree::actualOne()(RETURN);
}

void output::returnNothing()
{
    DataTree::actualOne()(RETURN_NOTHING);
}

void output::initThisLevel(int offset, std::string const& name)
{
    DataTree::actualOne()(INITIALIZE_THIS_LEVEL, name, offset);
}

void output::addResEntry(int offset)
{
    DataTree::actualOne()(ADD_RES_ENTRY, offset);
}

void output::refLevel(int offset, int level, std::string const& type_name)
{
    DataTree::actualOne()(REFERENCE, type_name, level, offset);
}

void output::writeInt(platform::int_type value)
{
    DataTree::actualOne()(INTEGER, util::str(value));
}

void output::writeFloat(platform::float_type value)
{
    DataTree::actualOne()(FLOAT, util::str(value));
}

void output::writeBool(bool value)
{
    DataTree::actualOne()(BOOLEAN, util::str(value));
}

void output::writeOperator(std::string const& op)
{
    DataTree::actualOne()(OPERATOR, op);
}

void output::emptyList()
{
    DataTree::actualOne()(EMPTY_LIST);
}

void output::listBegin(int size, std::string const& member_type_exported_name)
{
    DataTree::actualOne()(LIST_BEGIN, member_type_exported_name, size);
}

void output::listNextMember()
{
    DataTree::actualOne()(LIST_NEXT_MEMBER);
}

void output::listEnd()
{
    DataTree::actualOne()(LIST_END);
}

void output::listAppendBegin()
{
    DataTree::actualOne()(LIST_APPEND_BEGIN);
}

void output::listAppendEnd()
{
    DataTree::actualOne()(LIST_APPEND_END);
}

void output::beginExpr()
{
    DataTree::actualOne()(EXPRESSION_BEGIN);
}

void output::endExpr()
{
    DataTree::actualOne()(EXPRESSION_END);
}

void output::branchIf()
{
    DataTree::actualOne()(BRANCH_IF);
}

void output::branchElse()
{
    DataTree::actualOne()(BRANCH_ELSE);
}

void output::blockBegin()
{
    DataTree::actualOne()(BLOCK_BEGIN);
}

void output::blockEnd()
{
    DataTree::actualOne()(BLOCK_END);
}

void output::endOfStatement()
{
    DataTree::actualOne()(END_OF_STATEMENT);
}

void output::beginWriterStmt()
{
    DataTree::actualOne()(WRITER_BEGIN);
}

void output::endWriterStmt()
{
    DataTree::actualOne()(WRITER_END);
}

void output::memberCallBegin(std::string const& member_name)
{
    DataTree::actualOne()(MEMBER_CALL_BEGIN, member_name);
}

void output::memberCallEnd()
{
    DataTree::actualOne()(MEMBER_CALL_END);
}

std::string Parameter::resEntry() const
{
    return PARAM + util::str(offset);
}

std::string ResourceParam::resEntry() const
{
    return RES_PARAM + util::str(offset);
}

std::string output::formFuncReferenceType(int size)
{
    return util::str(size);
}

std::string output::formType(std::string const& name)
{
    return name;
}

std::string output::formListType(std::string const& name)
{
    return "list [" + name + ']';
}

std::string output::emptyListType()
{
    return "empty list type";
}
