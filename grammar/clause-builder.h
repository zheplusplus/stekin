#ifndef __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__
#define __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__

#include <vector>
#include <string>

#include "expr-nodes.h"
#include "../util/pointer.h"
#include "../misc/pos-type.h"

namespace grammar {

    struct clause_builder {
        void add_arith(int indent_len, util::sptr<expr_base const>&& arith);
        void add_var_def(int indent_len, std::string const& name, util::sptr<expr_base const>&& init);
        void add_return(int indent_len, util::sptr<expr_base const>&& ret_val);
        void add_void_return(int indent_len, misc::pos_type const& pos);

        void add_func_def(int indent_len
                        , misc::pos_type const& pos
                        , std::string const& name
                        , std::vector<std::string> const& params);
        void add_if(int indent_len, util::sptr<expr_base const>&& condition);
        void add_if_not(int indent_len, util::sptr<expr_base const>&& condition);
        void add_else(int indent_len, misc::pos_type const& pos);
        void add_while(int indent_len, util::sptr<expr_base const>&& condition);

        void build_and_clear();
    };

}

#endif /* __STACKENING_GRAMMAR_CLAUSE_BUILDER_H__ */
