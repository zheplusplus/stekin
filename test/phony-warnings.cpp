#include <list>

#include "phony-warnings.h"

using namespace test;

static std::list<consq_branch_terminated_rec> consq_branch_terminated_recs;
static std::list<alter_branch_terminated_rec> alter_branch_terminated_recs;
static std::list<both_branches_terminated_rec> both_branches_terminated_recs;

void test::clear_warn()
{
    consq_branch_terminated_recs.clear();
    alter_branch_terminated_recs.clear();
    both_branches_terminated_recs.clear();
}

void warning::one_or_two_branches_terminated(
        util::sref<misc::position const> consq_term_pos_or_null_if_not_term
      , util::sref<misc::position const> alter_term_pos_or_null_if_not_term)
{
    if (bool(consq_term_pos_or_null_if_not_term) && bool(alter_term_pos_or_null_if_not_term)) {
        return both_branches_terminated_recs.push_back(
                both_branches_terminated_rec(consq_term_pos_or_null_if_not_term.cp()
                                           , alter_term_pos_or_null_if_not_term.cp()));
    }
    if (bool(consq_term_pos_or_null_if_not_term)) {
        return consq_branch_terminated_recs.push_back(
                consq_branch_terminated_rec(consq_term_pos_or_null_if_not_term.cp()));
    }
    alter_branch_terminated_recs.push_back(
                alter_branch_terminated_rec(alter_term_pos_or_null_if_not_term.cp()));
}

std::vector<consq_branch_terminated_rec> test::get_consq_branch_terminated()
{
    return std::vector<consq_branch_terminated_rec>(consq_branch_terminated_recs.begin()
                                                  , consq_branch_terminated_recs.end());
}

std::vector<alter_branch_terminated_rec> test::get_alter_branch_terminated()
{
    return std::vector<alter_branch_terminated_rec>(alter_branch_terminated_recs.begin()
                                                  , alter_branch_terminated_recs.end());
}

std::vector<both_branches_terminated_rec> test::get_both_branches_terminated()
{
    return std::vector<both_branches_terminated_rec>(both_branches_terminated_recs.begin()
                                                   , both_branches_terminated_recs.end());
}
