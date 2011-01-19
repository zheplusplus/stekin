#ifndef __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__
#define __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__

#include <list>
#include <string>

#include "node-base.h"
#include "acceptor.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct acceptor_stack {
        void add(int level, util::sptr<acceptor> acc);

        void next_stmt(int level, util::sptr<stmt_base const> stmt);
        void next_func(int level, util::sptr<func_def const> func);

        void match_else(int level, misc::pos_type const& pos);

        block pack_all();

        acceptor_stack();
    private:
        void _fill_to(int level, misc::pos_type const& pos);
        void _shrink_to(int level);
        void _prepare_level(int level, misc::pos_type const& pos);
    private:
        struct acceptor_of_pack
            : public acceptor
        {
            acceptor_of_pack()
                : acceptor(misc::pos_type(-1))
            {}

            void accept_stmt(util::sptr<stmt_base const> stmt);
            void accept_func(util::sptr<func_def const> func);

            void deliver_to(util::sref<acceptor>) {}

            block pack();
        private:
            block _pack;
        };

        std::list<util::sptr<acceptor>> _acceptors;
        util::sref<acceptor_of_pack> const _packer;

        util::sref<acceptor_of_pack> _prepare_first_acceptor();
    };

    struct clause_builder {
        void add_arith(int indent_len, util::sptr<expr_base const> arith);
        void add_var_def(int indent_len, std::string const& name, util::sptr<expr_base const> init);
        void add_return(int indent_len, util::sptr<expr_base const> ret_val);
        void add_return_nothing(int indent_len, misc::pos_type const& pos);

        void add_func_def(int indent_len
                        , misc::pos_type const& pos
                        , std::string const& name
                        , std::vector<std::string> const& params);
        void add_if(int indent_len, util::sptr<expr_base const> condition);
        void add_ifnot(int indent_len, util::sptr<expr_base const> condition);
        void add_else(int indent_len, misc::pos_type const& pos);
        void add_while(int indent_len, util::sptr<expr_base const> condition);

        util::sptr<proto::scope const> build_and_clear();
    private:
        acceptor_stack _stack;
    };

}

#endif /* __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__ */
