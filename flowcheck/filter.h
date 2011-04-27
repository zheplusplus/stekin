#ifndef __STEKIN_FLOWCHECK_FILTER_H__
#define __STEKIN_FLOWCHECK_FILTER_H__

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
                          , util::sptr<Expression const> init) = 0;
        void defFunc(misc::position const& pos
                   , std::string const& name
                   , std::vector<std::string> const& param_names
                   , util::sptr<Filter> body);
    protected:
        virtual void _defFunc(misc::position const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names
                            , Accumulator body) = 0;
    public:
        virtual util::sptr<Expression const> makeRef(misc::position const& pos
                                                   , std::string const& name) = 0;
        virtual util::sptr<Expression const> makeCall(
                                          misc::position const& pos
                                        , std::string const& name
                                        , std::vector<util::sptr<Expression const>> args) = 0;
        virtual util::sptr<Expression const> makeFuncReference(misc::position const& pos
                                                             , std::string const& name
                                                             , int param_count) = 0;
    public:
        Block deliver();
    protected:
        Accumulator _accumulator;
    };

}

#endif /* __STEKIN_FLOWCHECK_FILTER_H__ */
