#include <algorithm>

#include <proto/function.h>
#include <proto/stmt-nodes.h>
#include <proto/variable.h>

#include "stmt-nodes.h"
#include "function.h"
#include "filter.h"
#include "symbol-table.h"

using namespace flchk;

util::sptr<proto::Statement> Arithmetics::compile(util::sref<proto::Block> block
                                                , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Arithmetics(pos, expr->compile(block, st)));
}

util::sptr<proto::Statement> Branch::compile(util::sref<proto::Block> block
                                           , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Block> consq_block(new proto::Block);
    util::sptr<proto::Block> alter_block(new proto::Block);
    consequence.compile(*consq_block, st);
    alternative.compile(*alter_block, st);
    return util::mkptr(new proto::Branch(pos
                                       , predicate->compile(block, st)
                                       , std::move(consq_block)
                                       , std::move(alter_block)));
}

util::sptr<proto::Statement> VarDef::compile(util::sref<proto::Block> block
                                           , util::sref<SymbolTable> st) const
{
    util::sptr<proto::Expression const> init_value(init->compile(block, st));
    st->defVar(pos, name);
    return util::mkptr(new proto::VarDef(pos, name, std::move(init_value)));
}

util::sptr<proto::Statement> Return::compile(util::sref<proto::Block> block
                                           , util::sref<SymbolTable> st) const
{
    return util::mkptr(new proto::Return(pos, ret_val->compile(block, st)));
}

util::sptr<proto::Statement> ReturnNothing::compile(util::sref<proto::Block>
                                                  , util::sref<SymbolTable>) const
{
    return util::mkptr(new proto::ReturnNothing(pos));
}
