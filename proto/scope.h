#ifndef __STEKIN_PROTO_SCOPE_H__
#define __STEKIN_PROTO_SCOPE_H__

#include <string>
#include <vector>
#include <list>
#include <gmpxx.h>

#include "block.h"
#include "fwd-decl.h"
#include "../misc/pos-type.h"

namespace proto {

    struct scope {
        virtual ~scope() {}
    public:
        util::sptr<expr_base const> make_bool(misc::pos_type const& pos, bool value) const;
        util::sptr<expr_base const> make_int(misc::pos_type const& pos, mpz_class const& value) const;
        util::sptr<expr_base const> make_float(misc::pos_type const& pos, mpf_class const& value) const;
        virtual util::sptr<expr_base const> make_ref(misc::pos_type const& pos, std::string const& name) = 0;
        virtual util::sptr<expr_base const> make_call(misc::pos_type const& pos
                                                    , std::string const& name
                                                    , std::vector<util::sptr<expr_base const>> args) const = 0;
        virtual util::sptr<expr_base const> make_func_reference(misc::pos_type const& pos
                                                              , std::string const& name
                                                              , int param_count) const = 0;
        util::sptr<expr_base const> make_binary(misc::pos_type const& pos
                                              , util::sptr<expr_base const> lhs
                                              , std::string const& op
                                              , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_pre_unary(misc::pos_type const& pos
                                                 , std::string const& op
                                                 , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_conj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_disj(misc::pos_type const& pos
                                            , util::sptr<expr_base const> lhs
                                            , util::sptr<expr_base const> rhs) const;
        util::sptr<expr_base const> make_nega(misc::pos_type const& pos, util::sptr<expr_base const> rhs) const;
    public:
        virtual void def_var(misc::pos_type const& pos, std::string const& name) = 0;
        virtual util::sref<function> declare(misc::pos_type const& pos
                                           , std::string const& name
                                           , std::vector<std::string> const& param_names
                                           , bool contains_void_return) = 0;
        virtual util::sptr<scope> create_branch_scope() = 0;
    public:
        void add_stmt(util::sptr<stmt_base const> stmt);
    public:
        block deliver();
        std::list<util::sptr<stmt_base const>> const& get_stmts() const;
    protected:
        block _block;
    protected:
        scope() {}

        scope(scope&& rhs)
            : _block(std::move(rhs._block))
        {}

        scope(scope const&) = delete;
    };

}

#endif /* __STEKIN_PROTO_SCOPE_H__ */
