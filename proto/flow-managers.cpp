#include "flow-managers.h"

using namespace proto;

void flow_mgr::add_stmt(util::sptr<stmt_base const> stmt)
{
    _stmts_ref.push_back(std::move(stmt));
}

termination_status flow_mgr::termination() const
{
    return NO_EXPLICIT_TERMINATION;
}

void teminated_flow::add_stmt(util::sptr<stmt_base const>)
{
    _host_mgr.reset(new dummy_flow);
}

termination_status teminated_flow::termination() const
{
    return term_status;
}

termination_status dummy_flow::termination() const
{
    return NO_EXPLICIT_TERMINATION;
}
