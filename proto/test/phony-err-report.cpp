#include "phony-err-report.h"

using namespace test;

namespace {

    struct error_records {
        std::list<forbid_def_rec> forbid_var_def_recs;
        std::list<var_redef_rec> local_redefs;
        std::list<invalid_ref_rec> invalid_refs;

        std::list<forbid_def_rec> forbid_func_def_recs;
        std::list<func_redef_rec> local_func_redefs;
        std::list<func_redef_rec> func_shadow_external;
        std::list<func_nondef_rec> func_nondefs;

        std::list<ret_type_unresolvable_rec> ret_type_unresolvable_recs;

        bool has_err;

        error_records()
            : has_err(false)
        {}

        void clear()
        {
            forbid_var_def_recs.clear();
            local_redefs.clear();
            invalid_refs.clear();

            forbid_func_def_recs.clear();
            local_func_redefs.clear();
            func_shadow_external.clear();
            func_nondefs.clear();

            ret_type_unresolvable_recs.clear();

            has_err = false;
        }
    };

    error_records records;

}

bool proto::has_error()
{
    return records.has_err;
}

void proto::forbid_def_var(misc::pos_type const& pos, std::string const& name)
{
    records.has_err = true;
    records.forbid_var_def_recs.push_back(forbid_def_rec(pos, name));
}

void proto::var_already_in_local(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& name)
{
    records.has_err = true;
    records.local_redefs.push_back(var_redef_rec(prev_def_pos, this_def_pos, name));
}

void proto::var_ref_before_def(misc::pos_type const& def_pos
                             , std::list<misc::pos_type> const& ref_positions
                             , std::string const& name)
{
    records.has_err = true;
    records.invalid_refs.push_back(invalid_ref_rec(ref_positions.begin(), ref_positions.end(), def_pos, name));
}

void proto::forbid_def_func(misc::pos_type const& pos, std::string const& name)
{
    records.has_err = true;
    records.forbid_func_def_recs.push_back(forbid_def_rec(pos, name));
}

void proto::func_already_in_local(misc::pos_type const& prev_def_pos
                                , misc::pos_type const& this_def_pos
                                , std::string const& name
                                , int param_count)
{
    records.has_err = true;
    records.local_func_redefs.push_back(func_redef_rec(prev_def_pos, this_def_pos, name, param_count));
}

void proto::func_shadow_external(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& name
                               , int param_count)
{
    records.has_err = true;
    records.func_shadow_external.push_back(func_redef_rec(prev_def_pos, this_def_pos, name, param_count));
}

void proto::func_not_def(misc::pos_type const& call_pos, std::string const& name, int param_count)
{
    records.has_err = true;
    records.func_nondefs.push_back(func_nondef_rec(call_pos, name, param_count));
}

void proto::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    records.has_err = true;
    records.ret_type_unresolvable_recs.push_back(ret_type_unresolvable_rec(name, arg_count));
}

void test::clear_err()
{
    records.clear();
}

std::vector<forbid_def_rec> test::get_forbid_var_defs()
{
    return std::vector<forbid_def_rec>(records.forbid_var_def_recs.begin(), records.forbid_var_def_recs.end());
}

std::vector<var_redef_rec> test::get_local_redefs()
{
    return std::vector<var_redef_rec>(records.local_redefs.begin(), records.local_redefs.end());
}

std::vector<invalid_ref_rec> test::get_invalid_refs()
{
    return std::vector<invalid_ref_rec>(records.invalid_refs.begin(), records.invalid_refs.end());
}

std::vector<func_redef_rec> test::get_local_func_redefs()
{
    return std::vector<func_redef_rec>(records.local_func_redefs.begin(), records.local_func_redefs.end());
}

std::vector<func_redef_rec> test::get_func_shadow_external()
{
    return std::vector<func_redef_rec>(records.func_shadow_external.begin()
                                     , records.func_shadow_external.end());
}

std::vector<func_nondef_rec> test::get_func_nondefs()
{
    return std::vector<func_nondef_rec>(records.func_nondefs.begin(), records.func_nondefs.end());
}

std::vector<forbid_def_rec> test::get_forbid_func_defs()
{
    return std::vector<forbid_def_rec>(records.forbid_func_def_recs.begin()
                                     , records.forbid_func_def_recs.end());
}

std::vector<ret_type_unresolvable_rec> test::get_ret_type_unresolvables()
{
    return std::vector<ret_type_unresolvable_rec>(records.ret_type_unresolvable_recs.begin()
                                                , records.ret_type_unresolvable_recs.end());
}
