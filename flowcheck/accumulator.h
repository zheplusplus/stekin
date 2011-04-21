#ifndef __STEKIN_FLOW_CHECK_ACCUMULATOR_H__
#define __STEKIN_FLOW_CHECK_ACCUMULATOR_H__

#include <string>
#include <vector>

#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct accumulator {
        accumulator()
            : _contains_void_return(false)
            , _error_reported(false)
            , _termination_pos(NULL)
        {}

        accumulator(accumulator const&) = delete;

        accumulator(accumulator&& rhs)
            : _Block(std::move(rhs._Block))
            , _contains_void_return(rhs._contains_void_return)
            , _error_reported(rhs._error_reported)
            , _termination_pos(std::move(rhs._termination_pos))
        {}
    public:
        void add_func_ret(misc::position const& pos, util::sptr<Expression const> ret_val);
        void add_func_ret_nothing(misc::position const& pos);
        void add_arith(misc::position const& pos, util::sptr<Expression const> expr);

        void add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , accumulator consequence
                      , accumulator alternative);

        void add_branch(misc::position const& pos
                      , util::sptr<Expression const> predicate
                      , accumulator consequence);

        void add_branch_alt_only(misc::position const& pos
                               , util::sptr<Expression const> predicate
                               , accumulator alternative);

        void add_Block(accumulator b);
    public:
        void def_var(misc::position const& pos, std::string const& name, util::sptr<Expression const> init);

        void def_func(misc::position const& pos
                    , std::string const& name
                    , std::vector<std::string> const& param_names
                    , accumulator body);
    public:
        Block deliver();
    private:
        void _set_terminated_by_void_return(misc::position const& pos);
        void _set_terminated_not_by_void_return(misc::position const& pos);
        void _set_termination_by_sub_accumulator(accumulator const& sub);

        static void _check_branches_temination(accumulator const& consequence, accumulator const& alternative);
        void _check_not_terminated(misc::position const& pos);

        void _report_terminated(misc::position const& pos);
    private:
        Block _Block;
        bool _contains_void_return;
        bool _error_reported;
        util::sptr<misc::position> _termination_pos;

        bool _terminated() const;
        void _set_self_terminated(accumulator term);
    };

}

#endif /* __STEKIN_FLOW_CHECK_ACCUMULATOR_H__ */
