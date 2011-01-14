#include "block.h"
#include "stmt-nodes.h"

namespace {

    struct dummy_flow
        : public flow_mgr
    {
        util::sptr<flow_mgr> block_base::flow_mgr::add_stmt(util::sptr<stmt_base const>) {}

        termination_status block_base::flow_mgr::termination() const
        {
            return NO_EXPLICIT_TERMINATION;
        }
    };

    struct teminated_flow
        : public flow_mgr
    {
        explicit teminated_flow(termination_status status, util::sptr<flow_mgr>& hostmgr)
            : term_status(status)
            , host_mgr(hostmgr)
        {}

        util::sptr<flow_mgr> block_base::flow_mgr::add_stmt(util::sptr<stmt_base const>)
        {
            host_mgr->reset(new dummy_flow(term_status));
        }

        termination_status block_base::flow_mgr::termination() const
        {
            return term_status;
        }

        termination_status const term_status;
        util::sptr<flow_mgr>& host_mgr;
    };

}

util::sptr<flow_mgr> block_base::flow_mgr::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts_ref->push_back(std::move(stmt));
}

termination_status block_base::flow_mgr::termination() const
{
    return NO_EXPLICIT_TERMINATION;
}

void block_base::func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(new func_ret(pos, std::move(ret_val))))));
    _flow_mgr.reset(new terminated_flow(RETURN_NO_VOID, _flow_mgr));
}

void block_base::func_ret_nothing(misc::pos_type const& pos)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(new func_ret(pos)))));
    _flow_mgr.reset(new terminated_flow(RETURN_VOID, _flow_mgr));
}

void block_base::make_arith(misc::pos_type const& pos, util::sptr<expr_base const> expr)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(new arithmetics(pos, std::move(expr))))));
}

void block_base::branch(misc::pos_type const& pos
                      , util::sptr<expr_base const> condition
                      , util::sptr<block_base const> valid
                      , util::sptr<block_base const> invalid)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(new branch(pos
                                                                 , std::move(condition)
                                                                 , std::move(valid._stmts)
                                                                 , std::move(invalid._stmts))))));
}

void block_base::loop(misc::pos_type const& pos
                , util::sptr<expr_base const> condition
                , util::sptr<block_base const> body)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(
                                new loop(pos, std::move(condition), std::move(body._stmts))))));
}

void block_base::def_var(misc::pos_type const& pos
                       , std::string const& name
                       , util::sptr<expr_base const> init)
{
    std::move(_flow_mgr->add_stmt(std::move(util::mkptr(new var_def(pos, name, std::move(init))))));
}

void global_block::func_ret(misc::pos_type const& pos, util::sptr<expr_base const> ret_val);
void global_block::func_ret_nothing(misc::pos_type const& pos);

util::sptr<block_base> sub_block() const
{
    return std::move(util::mkmptr(new global_sub_block));
}
    };

    struct global_sub_block
        : public global_block
    {
        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);
    };

    struct func_block
        : public block_base
    {
        util::sptr<block_base> sub_block() const;
    };

    struct func_sub_block
        : public func_block
    {
        void def_var(misc::pos_type const& pos, std::string const& name, util::sptr<expr_base const> init);
    };
