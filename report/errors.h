#ifndef __STEKIN_REPORT_ERRORS_H__
#define __STEKIN_REPORT_ERRORS_H__

#include <list>
#include <string>

#include "../misc/pos-type.h"

namespace error {

    bool hasError();

    void tabAsIndent(misc::position const& pos);
    void badIndent(misc::position const& pos);
    void invalidChar(misc::position const& pos, int character);

    void elseNotMatchIf(misc::position const& else_pos);
    void ifAlreadyMatchElse(misc::position const& prev_else_pos, misc::position const& this_else_pos);
    void excessiveIndent(misc::position const& pos);

    void flowTerminated(misc::position const& this_pos, misc::position const& prev_pos);

    void forbidDefFunc(misc::position const& pos, std::string const& name);
    void forbidDefVar(misc::position const& pos, std::string const& name);

    void var_already_in_local(misc::position const& prev_def_pos
                            , misc::position const& this_def_pos
                            , std::string const& var_name);
    void var_ref_before_def(misc::position const& def_pos
                          , std::list<misc::position> const& ref_positions
                          , std::string const& name);

    void funcReferenceAmbiguous(misc::position const& pos, std::string const& name);
    void func_already_def(misc::position const& prev_def_pos
                        , misc::position const& this_def_pos
                        , std::string const& name
                        , int param_count);
    void func_not_def(misc::position const& ref_pos, std::string const& name, int param_count);

    void var_not_def(misc::position const& ref_pos, std::string const& name);

    void binaryOpNotAvai(misc::position const& pos
                          , std::string const& op_img
                          , std::string const& lhst_name
                          , std::string const& rhst_name);
    void PreUnaryOp_not_avai(misc::position const& pos
                             , std::string const& op_img
                             , std::string const& rhst_name);

    void conflict_return_type(misc::position const& this_pos
                            , std::string const& prev_ret_type_name
                            , std::string const& this_ret_type_name);

    void returnTypeUnresolvable(std::string const& name, int arg_count);

    void condNotBool(misc::position const& pos, std::string const& actual_type);

    void request_variable_not_callable(misc::position const& call_pos);

}

#endif /* __STEKIN_REPORT_ERRORS_H__ */
