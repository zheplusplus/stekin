#ifndef __STEKIN_FLOW_CHECK_FILTER_H__
#define __STEKIN_FLOW_CHECK_FILTER_H__

#include <string>
#include <vector>

#include "accumulator.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct Filter {
        Filter() = default;
        Filter(Filter const&) = delete;

        Filter(Filter&& rhs)
            : _accumulator(std::move(rhs._accumulator))
        {}

        virtual ~Filter() {}
    public:
        void addReturn(misc::position const& pos, util::sptr<Expression const> ret_val);
        void addReturnNothing(misc::position const& pos);
        void addArith(misc::position const& pos, util::sptr<Expression const> expr);

        void addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence
                     , util::sptr<Filter> alternative);

        void addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , util::sptr<Filter> consequence);

        void addBranchAlterOnly(misc::position const& pos
                              , util::sptr<Expression const> predicate
                              , util::sptr<Filter> alternative);
    public:
        virtual void defVar(misc::position const& pos
                          , std::string const& name
                          , util::sptr<Expression const> init);

        virtual void defFunc(misc::position const& pos
                           , std::string const& name
                           , std::vector<std::string> const& param_names
                           , util::sptr<Filter> body);
    public:
        Block deliver();
    protected:
        Accumulator _accumulator;
    };

    struct SymbolDefFilter
        : public Filter
    {
        void defVar(misc::position const& pos
                  , std::string const& name
                  , util::sptr<Expression const>);

        void defFunc(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const&
                   , util::sptr<Filter>);
    };

}

#endif /* __STEKIN_FLOW_CHECK_FILTER_H__ */
