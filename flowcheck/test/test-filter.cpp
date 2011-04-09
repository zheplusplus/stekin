#include <vector>
#include <gtest/gtest.h>

#include "test-common.h"
#include "../expr-nodes.h"
#include "../filter.h"
#include "../function.h"
#include "../../proto/node-base.h"
#include "../../proto/function.h"
#include "../../proto/global-scope.h"
#include "../../instance/node-base.h"
#include "../../instance/inst-mediate.h"
#include "../../test/phony-errors.h"
#include "../../test/common.h"

using namespace test;

typedef flowcheck_test FilterTest;

TEST_F(FilterTest, NonTerminatedFilter)
{
    misc::pos_type pos(1);
    util::sptr<proto::scope> scope(std::move(new proto::global_scope));

    util::sptr<flchk::binary_op> binary(
            new flchk::binary_op(pos
                               , std::move(util::mkptr(new flchk::int_literal(pos, "1")))
                               , "+"
                               , std::move(util::mkptr(new flchk::float_literal(pos, "235.7")))));

    flchk::filter filter0;
    filter0.add_arith(pos, std::move(util::mkptr(new flchk::bool_literal(pos, true))));
    filter0.def_var(pos, "soujirou", std::move(binary));
    filter0.def_var(pos, "iwasaki", std::move(util::mkptr(new flchk::reference(pos, "minami"))));

    util::sptr<flchk::filter> filter_consq(new flchk::filter);
    misc::pos_type pos_consq(100);
    filter_consq->add_arith(pos_consq, std::move(util::mkptr(new flchk::reference(pos_consq, "kobayakawa"))));

    util::sptr<flchk::filter> filter_alter(new flchk::filter);
    misc::pos_type pos_alter(101);
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "yutaka"))));
    filter_alter->add_arith(pos_alter, std::move(util::mkptr(new flchk::reference(pos_alter, "hiyori"))));

    filter0.add_branch(pos
                     , std::move(util::mkptr(new flchk::reference(pos, "tamura")))
                     , std::move(filter_consq)
                     , std::move(filter_alter));
    filter0.deliver().compile(*scope);
    scope->deliver().inst(nul_inst_scope);

    EXPECT_FALSE(error::has_error());

    data_tree::expect_one()
        (pos, SCOPE_VAR_DEF, "soujirou")
        (pos, SCOPE_VAR_DEF, "iwasaki")
        (SCOPE_BEGIN)
            (pos, ARITHMETICS)
                (pos, BOOLEAN, "true")
            (pos, VAR_DEF, "soujirou")
                (pos, FLOATING, "236.7")
            (pos, VAR_DEF, "iwasaki")
                (pos, REFERENCE, "minami")
            (pos, BRANCH)
            (pos, REFERENCE, "tamura")
            (SCOPE_BEGIN)
                (pos_consq, ARITHMETICS)
                    (pos_consq, REFERENCE, "kobayakawa")
            (SCOPE_END)
            (SCOPE_BEGIN)
                (pos_alter, ARITHMETICS)
                    (pos_alter, REFERENCE, "yutaka")
                (pos_alter, ARITHMETICS)
                    (pos_alter, REFERENCE, "hiyori")
            (SCOPE_END)
        (SCOPE_END)
    ;
}
