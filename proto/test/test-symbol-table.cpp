#include <gtest/gtest.h>

#include "test-common.h"
#include "../symbol-table.h"
#include "../function.h"
#include "../node-base.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct SymbolTableTest
    : public proto_test
{
    void SetUp()
    {
        proto_test::SetUp();
        symbols.reset(new proto::SymbolTable);
        inst_scope.reset(new phony_func);
    }

    util::sref<proto::SymbolTable const> ref_sym()
    {
        return *symbols;
    }

    util::sptr<proto::SymbolTable> symbols;
    util::sptr<inst::Scope> inst_scope;
};

TEST_F(SymbolTableTest, DefVar)
{
    misc::position pos(1);
    symbols->defVar(pos, "nerv");
    symbols->defVar(pos, "seele");
    symbols->defVar(pos, "lilith");
    ASSERT_FALSE(error::hasError());
    proto::SymbolTable inner_symbols(ref_sym());
    inner_symbols.defVar(pos, "nerv");
    inner_symbols.defVar(pos, "seele");
    inner_symbols.defVar(pos, "adam");
    inner_symbols.defVar(pos, "eve");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, RefLocalVar)
{
    misc::position pos(2);
    symbols->defVar(pos, "nerv");
    symbols->defVar(pos, "seele");
    symbols->defVar(pos, "lilith");

    symbols->ref_var(pos, "nerv");
    symbols->ref_var(pos, "seele");
    symbols->ref_var(pos, "lilith");
    ASSERT_FALSE(error::hasError());

    proto::SymbolTable inner_symbols(ref_sym());
    inner_symbols.defVar(pos, "nerv");
    inner_symbols.defVar(pos, "seele");
    inner_symbols.defVar(pos, "adam");
    inner_symbols.defVar(pos, "eve");

    inner_symbols.ref_var(pos, "nerv");
    inner_symbols.ref_var(pos, "seele");
    inner_symbols.ref_var(pos, "adam");
    inner_symbols.ref_var(pos, "eve");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, RefExternal)
{
    misc::position pos(3);
    symbols->ref_var(pos, "soryu");
    symbols->ref_var(pos, "ikari");
    symbols->ref_var(pos, "horaki");
    ASSERT_FALSE(error::hasError());
    proto::SymbolTable inner_symbols(ref_sym());
    inner_symbols.ref_var(pos, "soryu");
    inner_symbols.ref_var(pos, "ikari");
    inner_symbols.ref_var(pos, "horaki");
    ASSERT_FALSE(error::hasError());
    util::sref<proto::SymbolTable const> inner_sym_ref(util::mkref(inner_symbols));
    proto::SymbolTable innest_symbols(inner_sym_ref);
    innest_symbols.ref_var(pos, "soryu");
    innest_symbols.ref_var(pos, "ikari");
    innest_symbols.ref_var(pos, "horaki");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, BindExternalVar)
{
    misc::position pos(4);
    misc::position bind_pos(100);
    std::map<std::string, inst::variable const> ext_refs;

    ext_refs = symbols->bind_external_var_refs(pos, *inst_scope);
    ASSERT_TRUE(ext_refs.empty());
    ASSERT_FALSE(error::hasError());

    symbols->ref_var(pos, "eva_00");
    symbols->ref_var(pos, "eva_01");
    symbols->ref_var(pos, "eva_02");
    ext_refs = symbols->bind_external_var_refs(bind_pos, *inst_scope);
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (bind_pos, QUERY_VAR, "eva_00")
        (bind_pos, QUERY_VAR, "eva_01")
        (bind_pos, QUERY_VAR, "eva_02")
    ;
}

TEST_F(SymbolTableTest, RedefVar)
{
    misc::position pos(5);
    misc::position err_pos0(200);
    misc::position err_pos1(201);

    symbols->defVar(pos, "suzuhara");
    symbols->defVar(pos, "aida");

    symbols->defVar(err_pos0, "suzuhara");
    symbols->defVar(err_pos1, "aida");
    ASSERT_TRUE(error::hasError());
    std::vector<var_redef_rec> redefs = get_local_redefs();
    ASSERT_EQ(2, redefs.size());
    ASSERT_EQ(pos, redefs[0].prev_pos);
    ASSERT_EQ(err_pos0, redefs[0].this_pos);
    ASSERT_EQ("suzuhara", redefs[0].name);
    ASSERT_EQ(pos, redefs[1].prev_pos);
    ASSERT_EQ(err_pos1, redefs[1].this_pos);
    ASSERT_EQ("aida", redefs[1].name);

    clear_err();
    proto::SymbolTable inner_symbols(ref_sym());
    inner_symbols.defVar(pos, "aida");
    inner_symbols.defVar(pos, "suzuhara");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, VarRefBeforeDef)
{
    misc::position pos(6);
    misc::position ref_pos0(300);
    misc::position ref_pos1(301);

    symbols->ref_var(ref_pos0, "katsuragi");
    symbols->ref_var(ref_pos1, "katsuragi");
    symbols->defVar(pos, "katsuragi");
    ASSERT_TRUE(error::hasError());
    std::vector<invalid_ref_rec> invalid_refs = get_invalid_refs();
    ASSERT_EQ(1, invalid_refs.size());
    ASSERT_EQ(pos, invalid_refs[0].def_pos);
    ASSERT_EQ(2, invalid_refs[0].ref_positions.size());
        ASSERT_EQ(ref_pos0, invalid_refs[0].ref_positions[0]);
        ASSERT_EQ(ref_pos1, invalid_refs[0].ref_positions[1]);
    ASSERT_EQ("katsuragi", invalid_refs[0].name);

    clear_err();
    symbols->ref_var(pos, "penpen");

    proto::SymbolTable inner_symbols(ref_sym());
    inner_symbols.ref_var(pos, "katsuragi");
    inner_symbols.defVar(pos, "penpen");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, DefFunc)
{
    misc::position pos(7);
    util::sref<proto::Function const> func(NULL);
    std::vector<std::string> param_names;
    func = symbols->defFunc(pos, "f0", param_names, true);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f0", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::hasError());
    func = symbols->defFunc(pos, "f1", param_names, true);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f1", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::hasError());
    param_names = { "x", "y", "z" };
    func = symbols->defFunc(pos, "f0", param_names, true);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f0", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::hasError());
    param_names.push_back("a");
    func = symbols->defFunc(pos, "f1", param_names, true);
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("f1", func->name);
    ASSERT_TRUE(param_names == func->param_names);
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, RefFunc)
{
    misc::position pos(8);
    misc::position ref_pos(400);
    util::sref<proto::Function const> func(NULL);
    std::vector<std::string> param_names;
    param_names = { "m", "n" };
    symbols->defFunc(pos, "fa", param_names, true);
    param_names = { "x", "y", "z" };
    symbols->defFunc(pos, "fa", param_names, true);
    symbols->defFunc(pos, "fb", param_names, true);

    func = symbols->query_func(ref_pos, "fa", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = symbols->query_func(ref_pos, "fa", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = symbols->query_func(ref_pos, "fb", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    proto::SymbolTable inner_symbols(ref_sym());
    param_names = { "a", "b" };
    inner_symbols.defFunc(pos, "fb", param_names, true);
    ASSERT_FALSE(error::hasError());

    func = inner_symbols.query_func(ref_pos, "fa", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fb", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "a", "b" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fa", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = inner_symbols.query_func(ref_pos, "fb", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);
}

TEST_F(SymbolTableTest, RedefFunc)
{
    misc::position pos(9);
    misc::position err_pos0(500);
    misc::position err_pos1(501);
    std::vector<std::string> param_names;

    param_names = { "m", "n" };
    symbols->defFunc(pos, "f0", param_names, true);
    param_names = { "x", "y", "z" };
    symbols->defFunc(pos, "f1", param_names, true);
    ASSERT_FALSE(error::hasError());

    param_names = { "a", "b" };
    symbols->defFunc(err_pos0, "f0", param_names, true);
    ASSERT_TRUE(error::hasError());
    std::vector<func_redef_rec> redefs = get_func_redefs();
    ASSERT_EQ(1, redefs.size());

    proto::SymbolTable inner_symbols(ref_sym());
    param_names = { "a", "b" };
    inner_symbols.defFunc(pos, "f1", param_names, true);

    param_names = { "a", "b", "c" };
    inner_symbols.defFunc(err_pos1, "f1", param_names, true);

    redefs = get_func_redefs();
    ASSERT_EQ(2, redefs.size());
    ASSERT_EQ(pos, redefs[0].prev_def_pos);
    ASSERT_EQ(err_pos0, redefs[0].this_def_pos);
    ASSERT_EQ("f0", redefs[0].name);
    ASSERT_EQ(2, redefs[0].param_count);
    ASSERT_EQ(pos, redefs[1].prev_def_pos);
    ASSERT_EQ(err_pos1, redefs[1].this_def_pos);
    ASSERT_EQ("f1", redefs[1].name);
    ASSERT_EQ(3, redefs[1].param_count);
}

TEST_F(SymbolTableTest, NondefFunc)
{
    misc::position pos(10);
    misc::position err_pos0(600);
    misc::position err_pos1(601);
    std::vector<func_nondef_rec> func_nondefs;
    std::vector<std::string> param_names;

    param_names = { "m", "n" };
    symbols->defFunc(pos, "f0", param_names, true);
    param_names = { "x", "y", "z" };
    symbols->defFunc(pos, "f1", param_names, true);
    ASSERT_FALSE(error::hasError());

    symbols->query_func(err_pos0, "f2", 3);
    ASSERT_TRUE(error::hasError());
    func_nondefs = get_func_nondefs();
    ASSERT_EQ(1, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);

    symbols->query_func(err_pos1, "f1", 2);
    ASSERT_TRUE(error::hasError());
    func_nondefs = get_func_nondefs();
    ASSERT_EQ(2, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);
    ASSERT_EQ(err_pos1, func_nondefs[1].call_pos);
    ASSERT_EQ("f1", func_nondefs[1].name);
    ASSERT_EQ(2, func_nondefs[1].param_count);
}

TEST_F(SymbolTableTest, References)
{
    misc::position pos(11);
    util::sptr<inst::Scope> inst_scope(new phony_func);

    symbols->defVar(pos, "otonashi");
    symbols->defVar(pos, "tachibana");
    ASSERT_FALSE(error::hasError());
    std::vector<std::string> param_names;
    param_names = { "yuzuru", "kanade" };
    symbols->defFunc(pos, "sss", param_names, true);

    symbols->ref_var(pos, "otonashi")->inst(*inst_scope)->typeof();
    symbols->ref_var(pos, "sss")->inst(*inst_scope)->typeof();
    symbols->ref_var(pos, "tachibana")->inst(*inst_scope)->typeof();
    ASSERT_FALSE(error::hasError());

    DataTree::expectOne()
        (pos, QUERY_VAR, "otonashi")
        (REFERENCE)
        (FUNC_REFERENCE)
        (pos, QUERY_VAR, "tachibana")
        (REFERENCE)
    ;
}

TEST_F(SymbolTableTest, FuncRefAmbiguous)
{
    misc::position pos(12);
    misc::position err_pos0(1200);
    misc::position err_pos1(1201);
    std::vector<std::string> param_names;
    param_names = { "nakamura" };
    symbols->defFunc(pos, "guild", param_names, true);
    symbols->defFunc(pos, "guild", std::vector<std::string>(), true);
    symbols->defFunc(pos, "girl_dead_monster", std::vector<std::string>(), true);
    proto::SymbolTable inner_symbols(ref_sym());
    param_names = { "yui", "iwasawa", "sekine" };
    symbols->defFunc(pos, "girl_dead_monster", param_names, true);

    symbols->ref_var(err_pos0, "guild");
    ASSERT_TRUE(error::hasError());
    symbols->ref_var(err_pos1, "girl_dead_monster");
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(2, getAmbiguousRefs().size());
    ASSERT_EQ("guild", getAmbiguousRefs()[0].name);
    ASSERT_EQ(err_pos0, getAmbiguousRefs()[0].ref_pos);
    ASSERT_EQ("girl_dead_monster", getAmbiguousRefs()[1].name);
    ASSERT_EQ(err_pos1, getAmbiguousRefs()[1].ref_pos);
}
