#ifndef __STEKIN_INSTANCE_LIST_PIPELINE_H__
#define __STEKIN_INSTANCE_LIST_PIPELINE_H__

#include <vector>

#include <util/pointer.h>

#include "node-base.h"
#include "types.h"

namespace inst {

    struct PipeBase {
        virtual ~PipeBase() {}

        explicit PipeBase(util::sptr<Expression const> e)
            : expr(std::move(e))
        {}

        virtual void begin() const = 0;
        virtual void end() const = 0;
        virtual void writeDef(int level) const = 0;

        util::sptr<Expression const> expr;
    };

    struct PipeMap
        : public PipeBase
    {
        PipeMap(util::sptr<Expression const> expr
              , util::sptr<Type const> st
              , util::sptr<Type const> dt)
            : PipeBase(std::move(expr))
            , src_member_type(std::move(st))
            , dst_member_type(std::move(dt))
        {}

        void begin() const;
        void end() const;
        void writeDef(int level) const;

        util::sptr<Type const> src_member_type;
        util::sptr<Type const> dst_member_type;
    };

    struct PipeFilter
        : public PipeBase
    {
        PipeFilter(util::sptr<Expression const> expr, util::sptr<Type const> mt)
            : PipeBase(std::move(expr))
            , member_type(std::move(mt))
        {}

        void begin() const;
        void end() const;
        void writeDef(int level) const;

        util::sptr<Type const> member_type;
    };

    struct ListPipeline
        : public Expression
    {
        ListPipeline(util::sptr<Expression const> l, std::vector<util::sptr<PipeBase const>> p)
            : list(std::move(l))
            , pipeline(std::move(p))
        {}

        void write() const;
        void writePipeDef(int level) const;

        util::sptr<Expression const> const list;
        std::vector<util::sptr<PipeBase const>> const pipeline;
    };

}

#endif /* __STEKIN_INSTANCE_LIST_PIPELINE_H__ */
