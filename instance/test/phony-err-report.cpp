#include <list>

#include "phony-err-report.h"

using namespace test;

namespace {

    struct error_records {
        std::list<ret_type_conflict_rec> ret_type_conflict_recs;
        std::list<ret_type_unresolvable_rec> ret_type_unresolvable_recs;
        std::list<cond_not_bool_rec> cond_not_bool_recs;

        std::list<var_nondef_rec> var_nondefs;

        std::list<na_binary_op_rec> na_binary_ops;
        std::list<na_pre_unary_op_rec> na_pre_unary_ops;

        bool has_err;

        error_records()
            : has_err(false)
        {}

        void clear()
        {
            var_nondefs.clear();

            na_binary_ops.clear();
            na_pre_unary_ops.clear();

            ret_type_conflict_recs.clear();
            ret_type_unresolvable_recs.clear();

            cond_not_bool_recs.clear();

            has_err = false;
        }
    };

    error_records records;

}

bool inst::has_error()
{
    return records.has_err;
}

void inst::var_not_def(misc::pos_type const& ref_pos, std::string const& name)
{
    records.has_err = true;
    records.var_nondefs.push_back(var_nondef_rec(ref_pos, name));
}

void inst::binary_op_not_avai(misc::pos_type const& pos
                            , std::string const& op_img
                            , std::string const& lhst
                            , std::string const& rhst)
{
    records.has_err = true;
    records.na_binary_ops.push_back(na_binary_op_rec(pos, op_img, lhst, rhst));
}

void inst::pre_unary_op_not_avai(misc::pos_type const& pos, std::string const& op_img, std::string const& rhst)
{
    records.has_err = true;
    records.na_pre_unary_ops.push_back(na_pre_unary_op_rec(pos, op_img, rhst));
}

void inst::conflict_return_type(misc::pos_type const& this_pos
                              , std::string const& prev_type_name
                              , std::string const& this_type_name)
{
    records.has_err = true;
    records.ret_type_conflict_recs.push_back(ret_type_conflict_rec(this_pos, prev_type_name, this_type_name));
}

void inst::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    records.has_err = true;
    records.ret_type_unresolvable_recs.push_back(ret_type_unresolvable_rec(name, arg_count));
}

void inst::cond_not_bool(misc::pos_type const& pos, std::string const& actual_type)
{
    records.has_err = true;
    records.cond_not_bool_recs.push_back(cond_not_bool_rec(pos, actual_type));
}

void test::clear_err()
{
    records.clear();
}

std::vector<var_nondef_rec> test::get_nondefs()
{
    return std::vector<var_nondef_rec>(records.var_nondefs.begin(), records.var_nondefs.end());
}

std::vector<na_binary_op_rec> test::get_na_binary_ops()
{
    return std::vector<na_binary_op_rec>(records.na_binary_ops.begin(), records.na_binary_ops.end());
}

std::vector<na_pre_unary_op_rec> test::get_na_pre_unary_ops()
{
    return std::vector<na_pre_unary_op_rec>(records.na_pre_unary_ops.begin(), records.na_pre_unary_ops.end());
}

std::vector<ret_type_conflict_rec> test::get_ret_type_conflicts()
{
    return std::vector<ret_type_conflict_rec>(records.ret_type_conflict_recs.begin()
                                            , records.ret_type_conflict_recs.end());
}

std::vector<ret_type_unresolvable_rec> test::get_ret_type_unresolvables()
{
    return std::vector<ret_type_unresolvable_rec>(records.ret_type_unresolvable_recs.begin()
                                                , records.ret_type_unresolvable_recs.end());
}

std::vector<cond_not_bool_rec> test::get_cond_not_bools()
{
    return std::vector<cond_not_bool_rec>(records.cond_not_bool_recs.begin(), records.cond_not_bool_recs.end());
}
