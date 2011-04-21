#ifndef __STEKIN_FLOW_CHECK_FILTER_H__
#define __STEKIN_FLOW_CHECK_FILTER_H__

#include <string>
#include <vector>

#include "accumulator.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct filter {
        filter() = default;
        filter(filter const&) = delete;

        filter(filter&& rhs)
            : _accumulator(std::move(rhs._accumulator))
        {}

        virtual ~filter() {}
    public:
        void add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val);
        void add_func_ret_nothing(misc::position const& pos);
        void add_arith(misc::position const& pos, util::sptr<Expression const> expr);

        void add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence
                      , util::sptr<filter> alternative);

        void add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , util::sptr<filter> consequence);

        void add_branch_alt_only(misc::position const& pos
                               , util::sptr<Expression const> predicate
                               , util::sptr<filter> alternative);
    public:
        virtual void def_var(misc::position const& pos
                           , std::string const& name
                           , util::sptr<Expression const> init);

        virtual void def_func(misc::position const& pos
                            , std::string const& name
                            , std::vector<std::string> const& param_names
                            , util::sptr<filter> body);
    public:
        Block deliver();
    protected:
        accumulator _accumulator;
    };

    struct symbol_def_filter
        : public filter
    {
        void def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const>);

        void def_func(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const&
                    , util::sptr<filter>);
    };

}

#endif /* __STEKIN_FLOW_CHECK_FILTER_H__ */
