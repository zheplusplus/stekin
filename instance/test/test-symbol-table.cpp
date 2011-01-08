#include <string>
#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "test-common.h"
#include "../symbol-table.h"
#include "../../misc/platform.h"
#include "../../misc/pos-type.h"

using namespace test;

struct SymbolTableTest
    : public testing::Test
{
    void SetUp()
    {
        clear_err();
    }
};

template <typename _OS>
_OS& operator<<(_OS& os, inst::variable const& var)
{
    os << "variable pos=" << var.def_pos << " type=" << var.vtype->name << " offset=" << var.stack_offset
       << " level=" << var.level;
    return os;
}

TEST_F(SymbolTableTest, OnCorrect)
{
    inst::symbol_table st;
    inst::variable var_a(st.def_var(misc::pos_type(1), &WORD, "a"));
    int word_used = 0;

    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100001), "a"), var_a);
    ASSERT_EQ(0, var_a.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_a.stack_offset);
    ++word_used;

    inst::variable var_b(st.def_var(misc::pos_type(2), &WORD, "b"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100002), "b"), var_b);
    ASSERT_EQ(0, var_b.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_b.stack_offset);
    ++word_used;

    inst::variable var_aa(st.def_var(misc::pos_type(3), &WORD, "aa"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100003), "aa"), var_aa);
    ASSERT_EQ(0, var_aa.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_aa.stack_offset);
    ++word_used;

    inst::variable var_one_word(st.def_var(misc::pos_type(4), &DWORD, "one_dword"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100004), "one_dword"), var_one_word);
    ASSERT_EQ(0, var_one_word.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_one_word.stack_offset);
    word_used += 2;

    inst::variable var_another_dw(st.def_var(misc::pos_type(5), &DWORD, "another_dw"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100005), "another_dw"), var_another_dw);
    ASSERT_EQ(0, var_another_dw.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_another_dw.stack_offset);
    word_used += 2;

    inst::variable var_boolean_one(st.def_var(misc::pos_type(6), inst::type::BIT_BOOL, "boolean_one"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100006), "boolean_one"), var_boolean_one);
    ASSERT_EQ(0, var_boolean_one.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_boolean_one.stack_offset);

    inst::variable var_boolean_2(st.def_var(misc::pos_type(7), inst::type::BIT_BOOL, "boolean_2"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100007), "boolean_2"), var_boolean_2);
    ASSERT_EQ(0, var_boolean_2.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + 1, var_boolean_2.stack_offset);

    ++word_used;
    inst::variable var_align_word(st.def_var(misc::pos_type(8), &WORD, "ALIGN_WORD"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(100008), "ALIGN_WORD"), var_align_word);
    ASSERT_EQ(0, var_align_word.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_align_word.stack_offset);
    ++word_used;

    for (int i = 0; i < platform::WORD_LENGTH_INBYTE * 2; ++i) {
        std::string var_name(std::string("b_") + char('a' + i));
        inst::variable var_byte(st.def_var(misc::pos_type(8 + i), inst::type::BIT_BOOL, var_name));
        ASSERT_FALSE(inst::has_error());
        ASSERT_EQ(st.query_var(misc::pos_type(100008 + i), var_name), var_byte);
        ASSERT_EQ(0, var_byte.level);
        ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + i, var_byte.stack_offset);
    }
    word_used += 2;

    inst::variable var_a_half(st.def_var(misc::pos_type(1000), &HALFWORD, "a_half"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101000), "a_half"), var_a_half);
    ASSERT_EQ(0, var_a_half.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_a_half.stack_offset);

    inst::variable var_b_half(st.def_var(misc::pos_type(1001), &HALFWORD, "b_half"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101001), "b_half"), var_b_half);
    ASSERT_EQ(0, var_b_half.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + platform::WORD_LENGTH_INBYTE / 2
          , var_b_half.stack_offset);
    ++word_used;

    inst::variable var_boolean_a(st.def_var(misc::pos_type(1002), inst::type::BIT_BOOL, "boolean_a"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101002), "boolean_a"), var_boolean_a);
    ASSERT_EQ(0, var_boolean_a.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_boolean_a.stack_offset);

    inst::variable var_boolean_b(st.def_var(misc::pos_type(1003), inst::type::BIT_BOOL, "boolean_b"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101003), "boolean_b"), var_boolean_b);
    ASSERT_EQ(0, var_boolean_b.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + 1, var_boolean_b.stack_offset);

    inst::variable var_c_half(st.def_var(misc::pos_type(1004), &HALFWORD, "c_half"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101004), "c_half"), var_c_half);
    ASSERT_EQ(0, var_c_half.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + 2, var_c_half.stack_offset);

    ++word_used;
    inst::variable var_d_half(st.def_var(misc::pos_type(1005), &HALFWORD, "d_half"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(st.query_var(misc::pos_type(101005), "d_half"), var_d_half);
    ASSERT_EQ(0, var_d_half.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used, var_d_half.stack_offset);

    for (int i = 0; i < platform::WORD_LENGTH_INBYTE / 2; ++i) {
        std::string var_name(std::string("bb_") + char('a' + i));
        inst::variable var_boolean(st.def_var(misc::pos_type(1005 + i), inst::type::BIT_BOOL, var_name));
        ASSERT_FALSE(inst::has_error());
        ASSERT_EQ(st.query_var(misc::pos_type(101005 + i), var_name), var_boolean);
        ASSERT_EQ(0, var_boolean.level);
        ASSERT_EQ(platform::WORD_LENGTH_INBYTE * word_used + platform::WORD_LENGTH_INBYTE / 2 + i
                , var_boolean.stack_offset);
    }
    ASSERT_FALSE(inst::has_error());
}

TEST_F(SymbolTableTest, RefNondefVar)
{
    inst::symbol_table st;
    std::vector<var_nondef_rec> nondef_errors;

    st.def_var(misc::pos_type(1), &WORD, "a");
    ASSERT_FALSE(inst::has_error());
    st.def_var(misc::pos_type(2), &WORD, "b");
    ASSERT_FALSE(inst::has_error());

    st.query_var(misc::pos_type(10001), "aa");
    ASSERT_TRUE(inst::has_error());
    st.query_var(misc::pos_type(10002), "bb");
    ASSERT_TRUE(inst::has_error());

    nondef_errors = get_nondefs();
    ASSERT_EQ(2, nondef_errors.size());

    ASSERT_EQ(misc::pos_type(10001), nondef_errors[0].ref_pos);
    ASSERT_EQ("aa", nondef_errors[0].name);
    ASSERT_EQ(misc::pos_type(10002), nondef_errors[1].ref_pos);
    ASSERT_EQ("bb", nondef_errors[1].name);

    st.query_var(misc::pos_type(10003), "c");
    ASSERT_TRUE(inst::has_error());

    nondef_errors = get_nondefs();
    ASSERT_EQ(3, nondef_errors.size());

    ASSERT_EQ(misc::pos_type(10001), nondef_errors[0].ref_pos);
    ASSERT_EQ("aa", nondef_errors[0].name);
    ASSERT_EQ(misc::pos_type(10002), nondef_errors[1].ref_pos);
    ASSERT_EQ("bb", nondef_errors[1].name);
    ASSERT_EQ(misc::pos_type(10003), nondef_errors[2].ref_pos);
    ASSERT_EQ("c", nondef_errors[2].name);
}

TEST_F(SymbolTableTest, NestedOnCorrect)
{
    inst::symbol_table ext_st;

    inst::variable alice(ext_st.def_var(misc::pos_type(1), &WORD, "alice"));
    inst::variable bob(ext_st.def_var(misc::pos_type(2), &WORD, "bob"));
    inst::variable claire(ext_st.def_var(misc::pos_type(3), &WORD, "claire"));
    inst::variable david(ext_st.def_var(misc::pos_type(3), &WORD, "david"));
    ASSERT_FALSE(inst::has_error());

    std::vector<inst::arg_name_type_pair> args_info_a = {};
    std::map<std::string, inst::variable const> ext_vars = {
        { "alice", alice },
        { "bob", bob },
        { "claire", claire },
        { "david", david },
    };

    int func_word_used = 0;
    inst::symbol_table sta(&ext_st, args_info_a, ext_vars);
    ASSERT_EQ(1, sta.level);

    ASSERT_EQ(sta.query_var(misc::pos_type(10000), "alice"), alice);
    ASSERT_EQ(sta.query_var(misc::pos_type(10001), "bob"), bob);
    ASSERT_EQ(sta.query_var(misc::pos_type(10002), "claire"), claire);
    ASSERT_EQ(sta.query_var(misc::pos_type(10003), "david"), david);
    ASSERT_FALSE(inst::has_error());

    inst::variable alicef(sta.def_var(misc::pos_type(10), &HALFWORD, "alice"));
    inst::variable bobf(sta.def_var(misc::pos_type(11), &HALFWORD, "bob"));
    inst::variable clairef(sta.def_var(misc::pos_type(12), &HALFWORD, "claire"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_NE(sta.query_var(misc::pos_type(10004), "alice"), alice);
    ASSERT_NE(sta.query_var(misc::pos_type(10005), "bob"), bob);
    ASSERT_NE(sta.query_var(misc::pos_type(10006), "claire"), claire);
    ASSERT_EQ(sta.query_var(misc::pos_type(10007), "david"), david);

    ASSERT_EQ(sta.query_var(misc::pos_type(10008), "alice"), alicef);
    ASSERT_EQ(sta.query_var(misc::pos_type(10009), "bob"), bobf);
    ASSERT_EQ(sta.query_var(misc::pos_type(10010), "claire"), clairef);

    ASSERT_EQ(1, alicef.level);
    ASSERT_EQ(1, bobf.level);
    ASSERT_EQ(1, clairef.level);
    ASSERT_EQ(0, david.level);

    ASSERT_EQ(func_word_used, alicef.stack_offset);
    ASSERT_EQ(func_word_used + platform::WORD_LENGTH_INBYTE / 2, bobf.stack_offset);
    ASSERT_EQ(func_word_used + platform::WORD_LENGTH_INBYTE, clairef.stack_offset);
    func_word_used += 2;
    ASSERT_FALSE(inst::has_error());

    std::vector<inst::arg_name_type_pair> args_info_b = {
        inst::arg_name_type_pair("x", &HALFWORD),
        inst::arg_name_type_pair("y", &HALFWORD),
        inst::arg_name_type_pair("z", &HALFWORD),
    };
    ext_vars = {
        { "alice", alicef },
        { "bob", bobf },
        { "claire", clairef },
        { "david", david },
    };

    inst::symbol_table stb(&sta, args_info_b, ext_vars);
    ASSERT_EQ(2, stb.level);
    ASSERT_EQ(stb.query_var(misc::pos_type(11000), "alice"), alicef);
    ASSERT_EQ(stb.query_var(misc::pos_type(11001), "bob"), bobf);
    ASSERT_EQ(stb.query_var(misc::pos_type(11002), "claire"), clairef);
    ASSERT_EQ(stb.query_var(misc::pos_type(11003), "david"), david);
    inst::variable func_x(stb.query_var(misc::pos_type(11004), "x"));
    inst::variable func_y(stb.query_var(misc::pos_type(11005), "y"));
    inst::variable func_z(stb.query_var(misc::pos_type(11006), "z"));
    ASSERT_FALSE(inst::has_error());
    ASSERT_EQ(2, func_x.level);
    ASSERT_EQ(2, func_y.level);
    ASSERT_EQ(2, func_z.level);
    ASSERT_EQ(0, func_x.stack_offset);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE / 2, func_y.stack_offset);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE, func_z.stack_offset);

    func_word_used = 1;
    inst::variable e(stb.def_var(misc::pos_type(10), &HALFWORD, "e"));
    ASSERT_EQ(2, e.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * func_word_used + platform::WORD_LENGTH_INBYTE / 2, e.stack_offset);
    ++func_word_used;
    inst::variable f(stb.def_var(misc::pos_type(11), &HALFWORD, "f"));
    ASSERT_EQ(2, f.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * func_word_used, f.stack_offset);
    inst::variable g(stb.def_var(misc::pos_type(12), &HALFWORD, "g"));
    ASSERT_EQ(2, g.level);
    ASSERT_EQ(platform::WORD_LENGTH_INBYTE * func_word_used + platform::WORD_LENGTH_INBYTE / 2, g.stack_offset);
    ++func_word_used;

    ASSERT_FALSE(inst::has_error());
}
