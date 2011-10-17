#ifndef __STEKIN_PROTO_LIST_PIPELINE_H__
#define __STEKIN_PROTO_LIST_PIPELINE_H__

#include <vector>

#include "node-base.h"
#include "list-context.h"

namespace proto {

    struct PipeBase {
        virtual ~PipeBase() {}

        explicit PipeBase(util::sptr<Expression const> e)
            : expr(std::move(e))
        {}

        virtual util::sptr<inst::PipeBase const> inst(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const = 0;
        virtual util::sref<Type const> typeTransfer(util::sref<SymbolTable const> st
                                                  , util::sref<ListContext const> lc
                                                  , misc::trace& trace) const = 0;

        util::sptr<Expression const> expr;
    };

    struct PipeMap
        : public PipeBase
    {
        explicit PipeMap(util::sptr<Expression const> expr)
            : PipeBase(std::move(expr))
        {}

        util::sptr<inst::PipeBase const> inst(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const> lc
                                            , misc::trace& trace) const;
        util::sref<Type const> typeTransfer(util::sref<SymbolTable const> st
                                          , util::sref<ListContext const> lc
                                          , misc::trace& trace) const;
    };

    struct PipeFilter
        : public PipeBase
    {
        explicit PipeFilter(util::sptr<Expression const> expr)
            : PipeBase(std::move(expr))
        {}

        util::sptr<inst::PipeBase const> inst(util::sref<SymbolTable const> st
                                            , util::sref<ListContext const> lc
                                            , misc::trace& trace) const;
        util::sref<Type const> typeTransfer(util::sref<SymbolTable const>
                                          , util::sref<ListContext const> lc
                                          , misc::trace&) const;
    };

    struct ListPipeline
        : public Expression
    {
        ListPipeline(misc::position const& pos
                   , util::sptr<Expression const> l
                   , std::vector<util::sptr<PipeBase const>> p)
            : Expression(pos)
            , list(std::move(l))
            , pipeline(std::move(p))
        {}

        util::sref<Type const> type(util::sref<SymbolTable const> st, misc::trace& trace) const;
        util::sptr<inst::Expression const> inst(util::sref<SymbolTable const> st
                                              , misc::trace& trace) const;
        util::sref<Type const> typeAsPipe(util::sref<SymbolTable const> st
                                        , util::sref<ListContext const> lc
                                        , misc::trace& trace) const;
        util::sptr<inst::Expression const> instAsPipe(util::sref<SymbolTable const> st
                                                    , util::sref<ListContext const> lc
                                                    , misc::trace& trace) const;

        util::sptr<Expression const> const list;
        std::vector<util::sptr<PipeBase const>> const pipeline;
    };

}

#endif /* __STEKIN_PROTO_LIST_PIPELINE_H__ */
