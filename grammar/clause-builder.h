#ifndef __STEKIN_GRAMMAR_CLAUSE_BUILDER_H__
#define __STEKIN_GRAMMAR_CLAUSE_BUILDER_H__

#include <list>
#include <vector>
#include <string>

#include "acceptor.h"
#include "fwd-decl.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct AcceptorStack {
        void add(int level, util::sptr<Acceptor> acc);

        void next_stmt(int level, util::sptr<Statement const> stmt);
        void next_func(int level, util::sptr<Function const> func);

        void match_else(int level, misc::position const& pos);

        Block pack_all();

        AcceptorStack();
    private:
        void _fill_to(int level, misc::position const& pos);
        void _shrink_to(int level);
        void _prepare_level(int level, misc::position const& pos);
    private:
        struct AcceptorOfPack
            : public Acceptor
        {
            AcceptorOfPack()
                : Acceptor(misc::position(-1))
            {}

            void accept_stmt(util::sptr<Statement const> stmt);
            void accept_func(util::sptr<Function const> func);

            void deliver_to(util::sref<Acceptor>) {}

            Block pack();
        private:
            Block _pack;
        };

        std::list<util::sptr<Acceptor>> _Acceptors;
        util::sref<AcceptorOfPack> const _packer;

        util::sref<AcceptorOfPack> _prepare_first_Acceptor();
    };

    struct ClauseBuilder {
        void addArith(int indent_len, util::sptr<Expression const> arith);
        void addVarDef(int indent_len, std::string const& name, util::sptr<Expression const> init);
        void addReturn(int indent_len, util::sptr<Expression const> ret_val);
        void addReturnNothing(int indent_len, misc::position const& pos);

        void addFunction(int indent_len
                        , misc::position const& pos
                        , std::string const& name
                        , std::vector<std::string> const& params);
        void addIf(int indent_len, util::sptr<Expression const> condition);
        void addIfnot(int indent_len, util::sptr<Expression const> condition);
        void addElse(int indent_len, misc::position const& pos);

        flchk::Block build_and_clear();
    private:
        AcceptorStack _stack;
    };

}

#endif /* __STEKIN_GRAMMAR_CLAUSE_BUILDER_H__ */
