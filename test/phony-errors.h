#ifndef __STEKIN_TEST_PHONY_ERRORS_H__
#define __STEKIN_TEST_PHONY_ERRORS_H__

#include <vector>

#include "../report/errors.h"

namespace test {

    struct TabAsIndRec {
        explicit TabAsIndRec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct BadIndentRec {
        explicit BadIndentRec(misc::position const& ps)
            : pos(ps)
        {}

        misc::position const pos;
    };

    struct InvCharRec {
        InvCharRec(misc::position const& ps, int ch)
            : pos(ps)
            , character(ch)
        {}

        misc::position const pos;
        int const character;
    };

    struct ElseNotMatchRec {
        misc::position const pos;

        explicit ElseNotMatchRec(misc::position const& ps)
            : pos(ps)
        {}
    };

    struct IfMatchedRec {
        misc::position const prev_pos;
        misc::position const this_pos;

        IfMatchedRec(misc::position const& ppos, misc::position const& tpos)
            : prev_pos(ppos)
            , this_pos(tpos)
        {}
    };

    struct ExcessIndRec {
        misc::position const pos;

        explicit ExcessIndRec(misc::position const& ps)
            : pos(ps)
        {}
    };

    struct FlowTerminatedRec {
        misc::position const this_pos;
        misc::position const prev_pos;

        FlowTerminatedRec(misc::position const tpos, misc::position const& ppos)
            : this_pos(tpos)
            , prev_pos(ppos)
        {}
    };

    struct FuncForbiddenRec {
        misc::position const pos;
        std::string const name;

        FuncForbiddenRec(misc::position const& ps, std::string const& n)
            : pos(ps)
            , name(n)
        {}
    };

    struct VarRedefRec {
        misc::position const prev_pos;
        misc::position const this_pos;
        std::string const name;

        VarRedefRec(misc::position const& ppos, misc::position const& tpos, std::string const& n)
            : prev_pos(ppos)
            , this_pos(tpos)
            , name(n)
        {}
    };

    struct InvalidRefRec {
        std::vector<misc::position> const ref_positions;
        misc::position const def_pos;
        std::string const name;

        template <typename _LineNoIterator>
        InvalidRefRec(_LineNoIterator pos_begin
                    , _LineNoIterator pos_end
                    , misc::position def_ps
                    , std::string const& n)
            : ref_positions(pos_begin, pos_end)
            , def_pos(def_ps)
            , name(n)
        {}
    };

    struct FuncRefAmbiguousRec {
        misc::position const ref_pos;
        std::string const name;

        FuncRefAmbiguousRec(misc::position const& rpos, std::string const& n)
            : ref_pos(rpos)
            , name(n)
        {}
    };

    struct FuncRedefRec {
        misc::position const prev_def_pos;
        misc::position const this_def_pos;
        std::string const name;
        int const param_count;

        FuncRedefRec(misc::position prev_def_ps
                   , misc::position this_def_ps
                   , std::string const& n
                   , int pc)
            : prev_def_pos(prev_def_ps)
            , this_def_pos(this_def_ps)
            , name(n)
            , param_count(pc)
        {}
    };

    struct FuncNondefRec {
        misc::position const call_pos;
        std::string const name;
        int const param_count;

        FuncNondefRec(misc::position cp, std::string const& n, int pc)
            : call_pos(cp)
            , name(n)
            , param_count(pc)
        {}
    };

    struct ForbidDefRec {
        misc::position const pos;
        std::string const name;

        ForbidDefRec(misc::position ps, std::string const& n)
            : pos(ps)
            , name(n)
        {}
    };

    struct VarNondefRec {
        misc::position const ref_pos;
        std::string const name;

        VarNondefRec(misc::position const& rpos, std::string const& n)
            : ref_pos(rpos)
            , name(n)
        {}
    };

    struct NABinaryOpRec {
        misc::position const pos;
        std::string const op_img;
        std::string const lhst_name;
        std::string const rhst_name;

        NABinaryOpRec(misc::position const& ps
                    , std::string const& op
                    , std::string const& lhst
                    , std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , lhst_name(lhst)
            , rhst_name(rhst)
        {}
    };

    struct NAPreUnaryOpRec {
        misc::position const pos;
        std::string const op_img;
        std::string const rhst_name;

        NAPreUnaryOpRec(misc::position const& ps, std::string const& op, std::string const& rhst)
            : pos(ps)
            , op_img(op)
            , rhst_name(rhst)
        {}
    };

    struct RetTypeConflictRec {
        misc::position const this_pos;
        std::string const prev_type_name;
        std::string const this_type_name;

        RetTypeConflictRec(misc::position const& tpos
                         , std::string const& prev_ret_type_name
                         , std::string const& this_ret_type_name)
            : this_pos(tpos)
            , prev_type_name(prev_ret_type_name)
            , this_type_name(this_ret_type_name)
        {}
    };

    struct RetTypeUnresolvableRec {
        std::string const name;
        int const arg_count;

        RetTypeUnresolvableRec(std::string const& n, int ac)
            : name(n)
            , arg_count(ac)
        {}
    };

    struct CondNotBoolRec {
        misc::position const pos;
        std::string const type_name;

        CondNotBoolRec(misc::position const& ps, std::string const& tname)
            : pos(ps)
            , type_name(tname)
        {}
    };

    struct VariableNotCallableRec {
        misc::position const call_pos;

        explicit VariableNotCallableRec(misc::position const& cp)
            : call_pos(cp)
        {}
    };

    struct VarCallArgCountWrong {
        misc::position const call_pos;
        int const actual;
        int const wanted;

        VarCallArgCountWrong(misc::position const& c, int a, int w)
            : call_pos(c)
            , actual(a)
            , wanted(w)
        {}
    };

    void clearErr();

    std::vector<TabAsIndRec> getTabAsIndents();
    std::vector<BadIndentRec> getBadIndents();
    std::vector<InvCharRec> getInvCharRecs();
    std::vector<ElseNotMatchRec> getElseNotMatches();
    std::vector<IfMatchedRec> getIfMatchedRecs();
    std::vector<ExcessIndRec> getExcessInds();
    std::vector<FlowTerminatedRec> getFlowTerminatedRecs();
    std::vector<FuncForbiddenRec> getForbiddenFuncs();
    std::vector<ForbidDefRec> getForbidVarDefs();
    std::vector<VarRedefRec> getLocalRedefs();
    std::vector<InvalidRefRec> getInvalidRefs();
    std::vector<FuncRefAmbiguousRec> getAmbiguousRefs();
    std::vector<FuncRedefRec> getFuncRedefs();
    std::vector<FuncNondefRec> getFuncNondefs();
    std::vector<VarNondefRec> get_nondefs();
    std::vector<NABinaryOpRec> getNABinaryOps();
    std::vector<NAPreUnaryOpRec> getNAPreUnaryOps();
    std::vector<RetTypeConflictRec> getRetTypeConflicts();
    std::vector<RetTypeUnresolvableRec> getRetTypeUnresolvables();
    std::vector<CondNotBoolRec> getCondNotBools();
    std::vector<VariableNotCallableRec> getVariableNotCallables();
    std::vector<VarCallArgCountWrong> getVarCallArgCountWrong();

}

#endif /* __STEKIN_TEST_PHONY_ERRORS_H__ */
