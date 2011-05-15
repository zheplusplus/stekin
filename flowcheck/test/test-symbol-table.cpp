#include <gtest/gtest.h>

#include "test-common.h"
#include "../symbol-table.h"
#include "../function.h"
#include "../func-body-filter.h"
#include "../node-base.h"
#include "../../proto/function.h"
#include "../../instance/node-base.h"
#include "../../test/common.h"
#include "../../test/phony-errors.h"

using namespace test;

struct SymbolTableTest
    : public FlowcheckTest
{
    void SetUp()
    {
        FlowcheckTest::SetUp();
        symbols.reset(new flchk::SymbolTable);
        scope.reset(new proto::Scope);
    }

    util::sref<flchk::SymbolTable const> refSym()
    {
        return *symbols;
    }

    util::sptr<flchk::Filter> mkBody()
    {
        return util::mkptr(new flchk::FuncBodyFilter(refSym()));
    }

    util::sptr<proto::Scope> scope;
    util::sptr<flchk::SymbolTable> symbols;
};

TEST_F(SymbolTableTest, DefVar)
{
    misc::position pos(1);
    symbols->defVar(pos, "nerv");
    symbols->defVar(pos, "seele");
    symbols->defVar(pos, "lilith");
    ASSERT_FALSE(error::hasError());
    flchk::SymbolTable inner_symbols(refSym());
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

    symbols->compileRef(pos, "nerv", *scope)->inst(nul_st);
    symbols->compileRef(pos, "seele", *scope)->inst(nul_st);
    symbols->compileRef(pos, "lilith", *scope)->inst(nul_st);
    ASSERT_FALSE(error::hasError());

    flchk::SymbolTable inner_symbols(refSym());
    inner_symbols.defVar(pos, "nerv");
    inner_symbols.defVar(pos, "seele");
    inner_symbols.defVar(pos, "adam");
    inner_symbols.defVar(pos, "eve");

    inner_symbols.compileRef(pos, "nerv", *scope)->inst(nul_st);
    inner_symbols.compileRef(pos, "seele", *scope)->inst(nul_st);
    inner_symbols.compileRef(pos, "adam", *scope)->inst(nul_st);
    inner_symbols.compileRef(pos, "eve", *scope)->inst(nul_st);
    ASSERT_FALSE(error::hasError());

    EXPECT_TRUE(symbols->freeVariables().empty());

    DataTree::expectOne()
        (pos, REFERENCE, "nerv")
        (pos, REFERENCE, "seele")
        (pos, REFERENCE, "lilith")

        (pos, REFERENCE, "nerv")
        (pos, REFERENCE, "seele")
        (pos, REFERENCE, "adam")
        (pos, REFERENCE, "eve")
    ;
}

TEST_F(SymbolTableTest, RefExternal)
{
    misc::position pos(3);
    symbols->compileRef(pos, "soryu", *scope)->inst(nul_st);
    symbols->compileRef(pos, "ikari", *scope)->inst(nul_st);
    symbols->compileRef(pos, "horaki", *scope)->inst(nul_st);
    ASSERT_FALSE(error::hasError());
    flchk::SymbolTable inner_symbols(refSym());
    inner_symbols.compileRef(pos, "soryu", *scope)->inst(nul_st);
    inner_symbols.compileRef(pos, "ikari", *scope)->inst(nul_st);
    inner_symbols.compileRef(pos, "horaki", *scope)->inst(nul_st);
    ASSERT_FALSE(error::hasError());
    util::sref<flchk::SymbolTable const> inner_sym_ref(util::mkref(inner_symbols));
    flchk::SymbolTable innest_symbols(inner_sym_ref);
    innest_symbols.compileRef(pos, "soryu", *scope)->inst(nul_st);
    innest_symbols.compileRef(pos, "ikari", *scope)->inst(nul_st);
    innest_symbols.compileRef(pos, "horaki", *scope)->inst(nul_st);
    ASSERT_FALSE(error::hasError());

    ASSERT_EQ(3, symbols->freeVariables().size());
    EXPECT_EQ("horaki", symbols->freeVariables()[0]);
    EXPECT_EQ("ikari", symbols->freeVariables()[1]);
    EXPECT_EQ("soryu", symbols->freeVariables()[2]);

    DataTree::expectOne()
        (pos, REFERENCE, "soryu")
        (pos, REFERENCE, "ikari")
        (pos, REFERENCE, "horaki")

        (pos, REFERENCE, "soryu")
        (pos, REFERENCE, "ikari")
        (pos, REFERENCE, "horaki")

        (pos, REFERENCE, "soryu")
        (pos, REFERENCE, "ikari")
        (pos, REFERENCE, "horaki")
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
    std::vector<VarRedefRec> redefs = getLocalRedefs();
    ASSERT_EQ(2, redefs.size());
    ASSERT_EQ(pos, redefs[0].prev_pos);
    ASSERT_EQ(err_pos0, redefs[0].this_pos);
    ASSERT_EQ("suzuhara", redefs[0].name);
    ASSERT_EQ(pos, redefs[1].prev_pos);
    ASSERT_EQ(err_pos1, redefs[1].this_pos);
    ASSERT_EQ("aida", redefs[1].name);

    clearErr();
    flchk::SymbolTable inner_symbols(refSym());
    inner_symbols.defVar(pos, "aida");
    inner_symbols.defVar(pos, "suzuhara");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, VarRefBeforeDef)
{
    misc::position pos(6);
    misc::position ref_pos0(300);
    misc::position ref_pos1(301);

    symbols->compileRef(ref_pos0, "katsuragi", *scope);
    symbols->compileRef(ref_pos1, "katsuragi", *scope);
    symbols->defVar(pos, "katsuragi");
    ASSERT_TRUE(error::hasError());
    std::vector<InvalidRefRec> invalid_refs = getInvalidRefs();
    ASSERT_EQ(1, invalid_refs.size());
    ASSERT_EQ(pos, invalid_refs[0].def_pos);
    ASSERT_EQ(2, invalid_refs[0].ref_positions.size());
        ASSERT_EQ(ref_pos0, invalid_refs[0].ref_positions[0]);
        ASSERT_EQ(ref_pos1, invalid_refs[0].ref_positions[1]);
    ASSERT_EQ("katsuragi", invalid_refs[0].name);

    clearErr();
    symbols->compileRef(pos, "penpen", *scope);

    flchk::SymbolTable inner_symbols(refSym());
    inner_symbols.compileRef(pos, "katsuragi", *scope);
    inner_symbols.defVar(pos, "penpen");
    ASSERT_FALSE(error::hasError());
}

TEST_F(SymbolTableTest, FuncDefRef)
{
    misc::position pos(8);
    misc::position ref_pos(400);
    std::vector<std::string> params;
    util::sref<flchk::Function> func(NULL);

    params = { "m", "n" };
    flchk::Function fa2(pos, "fa", params, mkBody());
    symbols->defFunc(util::mkref(fa2));
    params = { "x", "y", "z" };
    flchk::Function fa3(pos, "fa", params, mkBody());
    symbols->defFunc(util::mkref(fa3));
    flchk::Function fb(pos, "fb", params, mkBody());
    symbols->defFunc(util::mkref(fb));

    func = symbols->queryFunc(ref_pos, "fa", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = symbols->queryFunc(ref_pos, "fa", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = symbols->queryFunc(ref_pos, "fb", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    flchk::SymbolTable inner_symbols(refSym());
    params = { "a", "b" };
    flchk::Function fbi(pos, "fb", params, mkBody());
    inner_symbols.defFunc(util::mkref(fbi));
    ASSERT_FALSE(error::hasError());

    func = inner_symbols.queryFunc(ref_pos, "fa", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "m", "n" }) == func->param_names);

    func = inner_symbols.queryFunc(ref_pos, "fb", 2);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fb", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "a", "b" }) == func->param_names);

    func = inner_symbols.queryFunc(ref_pos, "fa", 3);
    ASSERT_FALSE(error::hasError());
    ASSERT_EQ(pos, func->pos);
    ASSERT_EQ("fa", func->name);
    ASSERT_TRUE(std::vector<std::string>({ "x", "y", "z" }) == func->param_names);

    func = inner_symbols.queryFunc(ref_pos, "fb", 3);
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
    std::vector<std::string> params;

    params = { "m", "n" };
    flchk::Function f0a(pos, "f0", params, mkBody());
    symbols->defFunc(util::mkref(f0a));
    params = { "x", "y", "z" };
    flchk::Function f1a(pos, "f1", params, mkBody());
    symbols->defFunc(util::mkref(f1a));
    ASSERT_FALSE(error::hasError());

    params = { "a", "b" };
    flchk::Function f0b(err_pos0, "f0", params, mkBody());
    symbols->defFunc(util::mkref(f0b));
    ASSERT_TRUE(error::hasError());
    std::vector<FuncRedefRec> redefs = getFuncRedefs();
    ASSERT_EQ(1, redefs.size());

    flchk::SymbolTable inner_symbols(refSym());
    params = { "a", "b" };
    flchk::Function f1b(pos, "f1", params, mkBody());
    inner_symbols.defFunc(util::mkref(f1b));

    params = { "a", "b", "c" };
    flchk::Function f1c(err_pos1, "f1", params, mkBody());
    inner_symbols.defFunc(util::mkref(f1c));

    redefs = getFuncRedefs();
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
    std::vector<FuncNondefRec> func_nondefs;
    std::vector<std::string> params;

    params = { "m", "n" };
    flchk::Function f0(pos, "f0", params, mkBody());
    symbols->defFunc(util::mkref(f0));
    params = { "x", "y", "z" };
    flchk::Function f1(pos, "f1", params, mkBody());
    symbols->defFunc(util::mkref(f1));
    ASSERT_FALSE(error::hasError());

    symbols->queryFunc(err_pos0, "f2", 3);
    ASSERT_TRUE(error::hasError());
    func_nondefs = getFuncNondefs();
    ASSERT_EQ(1, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);

    symbols->queryFunc(err_pos1, "f1", 2);
    ASSERT_TRUE(error::hasError());
    func_nondefs = getFuncNondefs();
    ASSERT_EQ(2, func_nondefs.size());
    ASSERT_EQ(err_pos0, func_nondefs[0].call_pos);
    ASSERT_EQ("f2", func_nondefs[0].name);
    ASSERT_EQ(3, func_nondefs[0].param_count);
    ASSERT_EQ(err_pos1, func_nondefs[1].call_pos);
    ASSERT_EQ("f1", func_nondefs[1].name);
    ASSERT_EQ(2, func_nondefs[1].param_count);
}

TEST_F(SymbolTableTest, FuncRefAmbiguous)
{
    misc::position pos(12);
    misc::position err_pos0(1200);
    misc::position err_pos1(1201);
    std::vector<std::string> params;
    params = { "nakamura" };
    flchk::Function ga(pos, "guild", params, mkBody());
    symbols->defFunc(util::mkref(ga));
    flchk::Function gb(pos, "guild", std::vector<std::string>(), mkBody());
    symbols->defFunc(util::mkref(gb));

    flchk::Function gdma(pos, "girl_dead_monster", std::vector<std::string>(), mkBody());
    symbols->defFunc(util::mkref(gdma));
    flchk::SymbolTable inner_symbols(refSym());
    params = { "yui", "iwasawa", "sekine" };
    flchk::Function gdmb(pos, "girl_dead_monster", params, mkBody());
    symbols->defFunc(util::mkref(gdmb));

    symbols->compileRef(err_pos0, "guild", *scope);
    ASSERT_TRUE(error::hasError());
    symbols->compileRef(err_pos1, "girl_dead_monster", *scope);
    ASSERT_TRUE(error::hasError());
    ASSERT_EQ(2, getAmbiguousRefs().size());
    ASSERT_EQ("guild", getAmbiguousRefs()[0].name);
    ASSERT_EQ(err_pos0, getAmbiguousRefs()[0].ref_pos);
    ASSERT_EQ("girl_dead_monster", getAmbiguousRefs()[1].name);
    ASSERT_EQ(err_pos1, getAmbiguousRefs()[1].ref_pos);

    DataTree::expectOne()
        (pos, FUNC_DECL, "guild", 0, true)
        (pos, FUNC_DECL, "girl_dead_monster", 0, true)
    ;
}
