#include <algorithm>

#include <output/func-writer.h>

#include "list-pipe.h"

using namespace inst;

void PipeMap::begin() const
{
    output::pipeBegin(util::id(this));
}

void PipeMap::end() const
{
    output::pipeEnd();
}

void PipeMap::writeDef(int level) const
{
    output::pipeMapBegin(util::id(this)
                       , level
                       , src_member_type->exportedName()
                       , dst_member_type->exportedName());
    expr->write();
    output::pipeMapEnd();
}

void PipeFilter::begin() const
{
    output::pipeBegin(util::id(this));
}

void PipeFilter::end() const
{
    output::pipeEnd();
}

void PipeFilter::writeDef(int level) const
{
    output::pipeFilterBegin(util::id(this), level, member_type->exportedName());
    expr->write();
    output::pipeFilterEnd();
}

void ListPipeline::write() const
{
    std::for_each(pipeline.rbegin()
                , pipeline.rend()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      pipe->begin();
                  });
    list->write();
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      pipe->end();
                  });
}

void ListPipeline::writePipeDef(int level) const
{
    std::for_each(pipeline.begin()
                , pipeline.end()
                , [&](util::sptr<PipeBase const> const& pipe)
                  {
                      pipe->writeDef(level);
                  });
}
