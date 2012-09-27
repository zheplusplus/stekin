#include <algorithm>

#include <instance/list-pipe.h>
#include <report/errors.h>

#include "list-pipe.h"
#include "list-types.h"

using namespace proto;

util::sptr<inst::PipeBase const> PipeMap::inst(util::sref<SymbolTable const> st
                                             , util::sref<ListContext const> lc
                                             , misc::trace& trace) const
{
    return util::mkptr(new inst::PipeMap(expr->instAsPipe(st, lc, trace)
                                       , lc->member_type->makeInstType()
                                       , typeTransfer(st, lc, trace)->makeInstType()));
}

util::sref<Type const> PipeMap::typeTransfer(util::sref<SymbolTable const> st
                                           , util::sref<ListContext const> lc
                                           , misc::trace& trace) const
{
    return expr->typeAsPipe(st, lc, trace);
}

util::sptr<inst::PipeBase const> PipeFilter::inst(util::sref<SymbolTable const> st
                                                , util::sref<ListContext const> lc
                                                , misc::trace& trace) const
{
    return util::mkptr(new inst::PipeFilter(expr->instAsPipe(st, lc, trace)
                                          , lc->member_type->makeInstType()));
}

util::sref<Type const> PipeFilter::typeTransfer(util::sref<SymbolTable const>
                                              , util::sref<ListContext const> lc
                                              , misc::trace&) const
{
    return lc->member_type;
}

static std::vector<util::sptr<inst::PipeBase const>> instPipeline(
                                          std::vector<util::sptr<PipeBase const>> const& pipeline
                                        , util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace)
{
    std::vector<util::sptr<inst::PipeBase const>> inst_pipe;
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      inst_pipe.push_back(pipe->inst(st, lc, trace));
                  });
    return std::move(inst_pipe);
}

static util::sref<Type const> typeTransfer(std::vector<util::sptr<PipeBase const>> const& pipeline
                                         , util::sref<Type const> member_type
                                         , util::sref<SymbolTable const> st
                                         , util::sref<ListContext const> lc
                                         , misc::trace& trace)
{
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      member_type = pipe->typeTransfer(st, lc, trace);
                  });
    return member_type;
}

util::sref<Type const> ListPipeline::type(util::sref<SymbolTable const> st
                                        , misc::trace& trace) const
{
    ListContext context(ListType::memberTypeOrNulIfNotList(list->type(st, trace)));
    if (context.member_type.nul()) {
        error::pipeNotApplyOnList(pos);
        return Type::bad();
    }
    return ListType::getListType(
                    typeTransfer(pipeline, context.member_type, st, util::mkref(context), trace));
}

util::sptr<inst::Expression const> ListPipeline::inst(util::sref<SymbolTable const> st
                                                    , misc::trace& trace) const
{
    ListContext context(ListType::memberTypeOrNulIfNotList(list->type(st, trace)));
    if (context.member_type.nul()) {
        error::pipeNotApplyOnList(pos);
        return util::mkptr(new inst::ListPipeline(list->inst(st, trace)
                                                , std::vector<util::sptr<inst::PipeBase const>>()));
    }
    return util::mkptr(new inst::ListPipeline(
                                        list->inst(st, trace)
                                      , instPipeline(pipeline, st, util::mkref(context), trace)));
}

util::sref<Type const> ListPipeline::typeAsPipe(util::sref<SymbolTable const> st
                                              , util::sref<ListContext const> lc
                                              , misc::trace& trace) const
{
    ListContext context(ListType::memberTypeOrNulIfNotList(list->typeAsPipe(st, lc, trace)));
    if (context.member_type.nul()) {
        error::pipeNotApplyOnList(pos);
        return Type::bad();
    }
    return ListType::getListType(
                    typeTransfer(pipeline, context.member_type, st, util::mkref(context), trace));
}

util::sptr<inst::Expression const> ListPipeline::instAsPipe(util::sref<SymbolTable const> st
                                                          , util::sref<ListContext const> lc
                                                          , misc::trace& trace) const
{
    ListContext context(ListType::memberTypeOrNulIfNotList(list->typeAsPipe(st, lc, trace)));
    if (context.member_type.nul()) {
        error::pipeNotApplyOnList(pos);
        return util::mkptr(new inst::ListPipeline(list->instAsPipe(st, lc, trace)
                                                , std::vector<util::sptr<inst::PipeBase const>>()));
    }
    return util::mkptr(new inst::ListPipeline(
                                        list->instAsPipe(st, lc, trace)
                                      , instPipeline(pipeline, st, util::mkref(context), trace)));
}
