#ifndef __STEKIN_FLOWCHECK_ACCUMULATOR_H__
#define __STEKIN_FLOWCHECK_ACCUMULATOR_H__

#include <string>
#include <vector>

#include "block.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace flchk {

    struct Accumulator {
        Accumulator()
            : _contains_void_return(false)
            , _error_reported(false)
            , _termination_pos(NULL)
        {}

        Accumulator(Accumulator const&) = delete;

        Accumulator(Accumulator&& rhs)
            : _block(std::move(rhs._block))
            , _contains_void_return(rhs._contains_void_return)
            , _error_reported(rhs._error_reported)
            , _termination_pos(std::move(rhs._termination_pos))
        {}
    public:
        void addReturn(misc::position const& pos, util::sptr<Expression const> ret_val);
        void addReturnNothing(misc::position const& pos);
        void addArith(misc::position const& pos, util::sptr<Expression const> expr);

        void addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , Accumulator consequence
                     , Accumulator alternative);

        void addBranch(misc::position const& pos
                     , util::sptr<Expression const> predicate
                     , Accumulator consequence);

        void addBranchAlterOnly(misc::position const& pos
                              , util::sptr<Expression const> predicate
                              , Accumulator alternative);

        void addBlock(Accumulator b);
    public:
        void defVar(misc::position const& pos
                  , util::sref<SymbolTable> symbols
                  , std::string const& name
                  , util::sptr<Expression const> init);

        util::sref<Function> defFunc(misc::position const& pos
                                   , std::string const& name
                                   , std::vector<std::string> const& param_names
                                   , util::sptr<Filter> body);
    public:
        bool containsVoidReturn() const;
        Block deliver();
    private:
        void _setTerminatedByVoidReturn(misc::position const& pos);
        void _setTerminatedNotByVoidReturn(misc::position const& pos);
        void _setTerminationBySubAccumulator(Accumulator const& sub);

        static void _checkBranchesTermination(Accumulator const& consequence
                                            , Accumulator const& alternative);
        void _checkNotTerminated(misc::position const& pos);

        void _reportTerminated(misc::position const& pos);

        bool _terminated() const;
        void _setSelfTerminated(Accumulator term);
    private:
        Block _block;
        bool _contains_void_return;
        bool _error_reported;
        util::sptr<misc::position> _termination_pos;
    };

}

#endif /* __STEKIN_FLOWCHECK_ACCUMULATOR_H__ */
