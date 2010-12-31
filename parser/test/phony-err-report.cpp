#include <list>
#include <string>

#include "phony-err-report.h"

using namespace test;

namespace {

    struct error_records {
        std::list<tab_as_ind_rec> tab_as_ind_recs;
        std::list<bad_indent_rec> bad_indent_recs;
        std::list<inv_char_rec> inv_char_recs;

        bool has_err;

        error_records()
            : has_err(false)
        {}

        void clear()
        {
            has_err = false;
            tab_as_ind_recs.clear();
            bad_indent_recs.clear();
            inv_char_recs.clear();
        }
    };

    error_records records;

}

void yyerror(std::string const&)
{
    records.has_err = true;
}

bool parser::has_error()
{
    return records.has_err;
}

void parser::tab_as_indent(misc::pos_type const& pos)
{
    records.has_err = true;
    records.tab_as_ind_recs.push_back(tab_as_ind_rec(pos));
}

void parser::bad_indent(misc::pos_type const& pos)
{
    records.has_err = true;
    records.bad_indent_recs.push_back(bad_indent_rec(pos));
}

void parser::invalid_char(misc::pos_type const& pos, int character)
{
    records.has_err = true;
    records.inv_char_recs.push_back(inv_char_rec(pos, character));
}

std::vector<tab_as_ind_rec> test::get_tab_as_ind_recs()
{
    return std::vector<tab_as_ind_rec>(records.tab_as_ind_recs.begin(), records.tab_as_ind_recs.end());
}

std::vector<bad_indent_rec> test::get_bad_indent_recs()
{
    return std::vector<bad_indent_rec>(records.bad_indent_recs.begin(), records.bad_indent_recs.end());
}

std::vector<inv_char_rec> test::get_inv_char_recs()
{
    return std::vector<inv_char_rec>(records.inv_char_recs.begin(), records.inv_char_recs.end());
}
