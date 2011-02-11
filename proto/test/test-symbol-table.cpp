#include <gtest/gtest.h>

#include "test-common.h"
#include "../symbol-table.h"
#include "../func-templ.h"
#include "../../test/phony-errors.h"

using namespace test;

struct SymbolTableTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        symbols.reset(new proto::symbol_table);
        inst_scope.reset(new phony_func);
    }

    util::sref<proto::symbol_table const> ref_sym()
    {
        return *symbols;
    }

    util::sptr<proto::symbol_table> symbols;
    util::sptr<inst::scope> inst_scope;
};

TEST_F(SymbolTableTest, DefVar)
{
    misc::pos_type pos(1);
    symbols->def_var(pos, "nerv");
    symbols->def_var(pos, "seele");
    symbols->def_var(pos, "lilith");
    ASSERT_FALSE(error::has_error());
    proto::symbol_table inner_symbols(ref_sym());
    inner_symbols.def_var(pos, "nerv");
    inner_symbols.def_var(pos, "seele");
    inner_symbols.def_var(pos, "adam");
    inner_symbols.def_var(pos, "eve");
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, RefLocalVar)
{
    misc::pos_type pos(2);
    symbols->def_var(pos, "nerv");
    symbols->def_var(pos, "seele");
    symbols->def_var(pos, "lilith");

    symbols->ref_var(pos, "nerv");
    symbols->ref_var(pos, "seele");
    symbols->ref_var(pos, "lilith");
    ASSERT_FALSE(error::has_error());

    proto::symbol_table inner_symbols(ref_sym());
    inner_symbols.def_var(pos, "nerv");
    inner_symbols.def_var(pos, "seele");
    inner_symbols.def_var(pos, "adam");
    inner_symbols.def_var(pos, "eve");

    inner_symbols.ref_var(pos, "nerv");
    inner_symbols.ref_var(pos, "seele");
    inner_symbols.ref_var(pos, "adam");
    inner_symbols.ref_var(pos, "eve");
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, RefExternal)
{
    misc::pos_type pos(3);
    symbols->ref_var(pos, "soryu");
    symbols->ref_var(pos, "ikari");
    symbols->ref_var(pos, "horaki");
    ASSERT_FALSE(error::has_error());
    proto::symbol_table inner_symbols(ref_sym());
    inner_symbols.ref_var(pos, "soryu");
    inner_symbols.ref_var(pos, "ikari");
    inner_symbols.ref_var(pos, "horaki");
    ASSERT_FALSE(error::has_error());
    util::sref<proto::symbol_table const> inner_sym_ref(util::mkref(inner_symbols));
    proto::symbol_table innest_symbols(inner_sym_ref);
    innest_symbols.ref_var(pos, "soryu");
    innest_symbols.ref_var(pos, "ikari");
    innest_symbols.ref_var(pos, "horaki");
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, BindExternalVar)
{
    misc::pos_type pos(4);
    misc::pos_type bind_pos(100);
    std::map<std::string, inst::variable const> ext_refs;

    ext_refs = symbols->bind_external_var_refs(pos, *inst_scope);
    ASSERT_TRUE(ext_refs.empty());
    ASSERT_FALSE(error::has_error());

    symbols->ref_var(pos, "eva_00");
    symbols->ref_var(pos, "eva_01");
    symbols->ref_var(pos, "eva_02");
    ext_refs = symbols->bind_external_var_refs(bind_pos, *inst_scope);
    ASSERT_FALSE(error::has_error());

    data_tree::expect_one()
        (bind_pos, QUERY_VAR, "eva_00")
        (bind_pos, QUERY_VAR, "eva_01")
        (bind_pos, QUERY_VAR, "eva_02")
    ;
}

TEST_F(SymbolTableTest, RedefVar)
{
    misc::pos_type pos(5);
    misc::pos_type err_pos0(200);
    misc::pos_type err_pos1(201);

    symbols->def_var(pos, "suzuhara");
    symbols->def_var(pos, "aida");

    symbols->def_var(err_pos0, "suzuhara");
    symbols->def_var(err_pos1, "aida");
    ASSERT_TRUE(error::has_error());
    std::vector<var_redef_rec> redefs = get_local_redefs();
    ASSERT_EQ(2, redefs.size());
    ASSERT_EQ(pos, redefs[0].prev_pos);
    ASSERT_EQ(err_pos0, redefs[0].this_pos);
    ASSERT_EQ("suzuhara", redefs[0].name);
    ASSERT_EQ(pos, redefs[1].prev_pos);
    ASSERT_EQ(err_pos1, redefs[1].this_pos);
    ASSERT_EQ("aida", redefs[1].name);

    clear_err();
    proto::symbol_table inner_symbols(ref_sym());
    inner_symbols.def_var(pos, "aida");
    inner_symbols.def_var(pos, "suzuhara");
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, VarRefBeforeDef)
{
    misc::pos_type pos(6);
    misc::pos_type ref_pos0(300);
    misc::pos_type ref_pos1(301);

    symbols->ref_var(ref_pos0, "katsuragi");
    symbols->ref_var(ref_pos1, "katsuragi");
    symbols->def_var(pos, "katsuragi");
    ASSERT_TRUE(error::has_error());
    std::vector<invalid_ref_rec> invalid_refs = get_invalid_refs();
    ASSERT_EQ(1, invalid_refs.size());
    ASSERT_EQ(pos, invalid_refs[0].def_pos);
    ASSERT_EQ(2, invalid_refs[0].ref_positions.size());
        ASSERT_EQ(ref_pos0, invalid_refs[0].ref_positions[0]);
        ASSERT_EQ(ref_pos1, invalid_refs[0].ref_positions[1]);
    ASSERT_EQ("katsuragi", invalid_refs[0].name);

    clear_err();
    symbols->ref_var(pos, "penpen");

    proto::symbol_table inner_symbols(ref_sym());
    inner_symbols.ref_var(pos, "katsuragi");
    inner_symbols.def_var(pos, "penpen");
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, DefFunc)
{
    misc::pos_type pos(7);
    util::sref<proto::func_templ const> func(NULL);
    std::vector<std::string> param_names;
    func = symbols->def_func(pos, "f0", param_names);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f0", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::has_error());
    func = symbols->def_func(pos, "f1", param_names);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f1", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::has_error());
    param_names = { "x", "y", "z" };
    func = symbols->def_func(pos, "f0", param_names);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f0", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::has_error());
    param_names.push_back("a");
    func = symbols->def_func(pos, "f1", param_names);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f1", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::has_error());
}

TEST_F(SymbolTableTest, RefFunc)
{
    misc::pos_type pos(8);
    misc::pos_type ref_pos(400);
    util::sref<proto::func_templ const> func(NULL);
    std::vector<std::string> param_names;
    param_names = { "m", "n" };
    symbols->def_func(pos, "fa", param_names);
    param_names = { "x", "y", "z" };
    symbols->def_func(pos, "fa", param_names);
    symbols->def_func(pos, "fb", param_names);

    func = symbols->query_func(ref_pos, "fa", 2);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = symbols->query_func(ref_pos, "fa", 3);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = symbols->query_func(ref_pos, "fb", 3);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    proto::symbol_table inner_symbols(ref_sym());
    param_names = { "a", "b" };
    inner_symbols.def_func(pos, "fb", param_names);
    ASSERT_FALSE(error::has_error());

    func = inner_symbols.query_func(ref_pos, "fa", 2);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fb", 2);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "a", "b" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fa", 3);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fb", 3);
    ASSERT_FALSE(error::has_error());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);
}

TEST_F(SymbolTableTest, RedefFunc)
{
    misc::pos_type pos(9);
    misc::pos_type err_pos0(500);
    misc::pos_type err_pos1(501);
    std::vector<std::string> param_names;

    param_names = { "m", "n" };
    symbols->def_func(pos, "f0", param_names);
    param_names = { "x", "y", "z" };
    symbols->def_func(pos, "f1", param_names);
    ASSERT_FALSE(error::has_error());

    param_names = { "a", "b" };
    symbols->def_func(err_pos0, "f0", param_names);
    ASSERT_TRUE(error::has_error());
    std::vector<func_redef_rec> local_redefs = get_local_func_redefs();
    ASSERT_EQ(1, local_redefs.size());
    ASSERT_EQ(pos, local_redefs[0].prev_def_pos);
    ASSERT_EQ(err_pos0, local_redefs[0].this_def_pos);
    ASSERT_EQ("f0", local_redefs[0].name);
    ASSERT_EQ(2, local_redefs[0].param_count);

    proto::symbol_table inner_symbols(ref_sym());
    param_names = { "a", "b" };
    inner_symbols.def_func(pos, "f1", param_names);

    param_names = { "a", "b", "c" };
    inner_symbols.def_func(err_pos1, "f1", param_names);

    std::vector<func_redef_rec> extern_shadow_defs = get_func_shadow_external();
    ASSERT_EQ(1, extern_shadow_defs.size());
    ASSERT_EQ(1, local_redefs.size());
    ASSERT_EQ(pos, extern_shadow_defs[0].prev_def_pos);
    ASSERT_EQ(err_pos1, extern_shadow_defs[0].this_def_pos);
    ASSERT_EQ("f1", extern_shadow_defs[0].name);
    ASSERT_EQ(3, extern_shadow_defs[0].param_count);
}

TEST_F(SymbolTableTest, NondefFunc)
{
    misc::pos_type pos(10);
    misc::pos_type err_pos0(600);
    misc::pos_type err_pos1(601);
    std::vector<func_nondef_rec> func_nondefs;
    std::vector<std::string> param_names;

    param_names = { "m", "n" };
    symbols->def_func(pos, "f0", param_names);
    param_names = { "x", "y", "z" };
    symbols->def_func(pos, "f1", param_names);
    ASSERT_FALSE(error::has_error());

    symbols->query_func(err_pos0, "f2", 3);
    ASSERT_TRUE(error::has_error());
    func_nondefs = get_func_nondefs();
    ASSERT_EQ(1, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);

    symbols->query_func(err_pos1, "f1", 2);
    ASSERT_TRUE(error::has_error());
    func_nondefs = get_func_nondefs();
    ASSERT_EQ(2, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);
    ASSERT_EQ(err_pos1, func_nondefs[1].call_pos);
    ASSERT_EQ("f1", func_nondefs[1].name);
    ASSERT_EQ(2, func_nondefs[1].param_count);
}
