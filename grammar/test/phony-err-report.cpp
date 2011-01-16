#include <list>

#include "phony-err-report.h"

using namespace test;

namespace {

    struct error_records {
        std::list<else_not_match_rec> else_not_matches_recs;
        std::list<if_matched_rec> if_matcheds_recs;
        std::list<excess_ind_rec> excess_ind_recs;
        std::list<func_forbidden_rec> forbidden_func_recs;

        bool has_err;

        error_records()
            : has_err(false)
        {}

        void clear()
        {
            else_not_matches_recs.clear();
            if_matcheds_recs.clear();
            excess_ind_recs.clear();
            forbidden_func_recs.clear();

            has_err = false;
        }
    };

    error_records records;

}

bool grammar::has_error()
{
    return records.has_err;
}

void grammar::else_not_match_if(misc::pos_type const& pos)
{
    records.has_err = true;
    records.else_not_matches_recs.push_back(else_not_match_rec(pos));
}

void grammar::if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos)
{
    records.has_err = true;
    records.if_matcheds_recs.push_back(if_matched_rec(prev_else_pos, this_else_pos));
}

void grammar::excessive_indent(misc::pos_type const& pos)
{
    records.has_err = true;
    records.excess_ind_recs.push_back(excess_ind_rec(pos));
}

void grammar::forbid_def_func(misc::pos_type const& pos, std::string const& name)
{
    records.has_err = true;
    records.forbidden_func_recs.push_back(func_forbidden_rec(pos, name));
}

void test::clear_err()
{
    records.clear();
}

std::vector<else_not_match_rec> test::get_else_not_matches()
{
    return std::vector<else_not_match_rec>(records.else_not_matches_recs.begin()
                                         , records.else_not_matches_recs.end());
}

std::vector<if_matched_rec> test::get_if_matcheds()
{
    return std::vector<if_matched_rec>(records.if_matcheds_recs.begin(), records.if_matcheds_recs.end());
}

std::vector<excess_ind_rec> test::get_excess_inds()
{
    return std::vector<excess_ind_rec>(records.excess_ind_recs.begin(), records.excess_ind_recs.end());
}

std::vector<func_forbidden_rec> test::get_forbidden_funcs()
{
    return std::vector<func_forbidden_rec>(records.forbidden_func_recs.begin()
                                         , records.forbidden_func_recs.end());
}
