#include <iostream>
#include <algorithm>

#include "errors.h"

static bool _has_error = false;

extern int yylineno;

bool error::has_error()
{
    return _has_error;
}

void yyerror(std::string const& msg)
{
    _has_error = true;
    std::cerr << "Line " << yylineno << ":" << std::endl;
    std::cerr << "    " <<  msg << std::endl;
}

void error::tab_as_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    use tab as indent is forbidden." << std::endl;
}

void error::bad_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    indent not exactly 4 spaces." << std::endl;
}

void error::invalid_char(misc::pos_type const& pos, int character)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    invalid character " << char(character)
              << " (decimal value: " << character << ')' << std::endl;
}

void error::else_not_match_if(misc::pos_type const& else_pos)
{
    _has_error = true;
    std::cerr << else_pos << std::endl;
    std::cerr << "    ``else'' does not match an ``if''" << std::endl;
}

void error::if_already_match_else(misc::pos_type const& prev_else_pos, misc::pos_type const& this_else_pos)
{
    _has_error = true;
    std::cerr << this_else_pos << std::endl;
    std::cerr << "    another ``else'' already matches the ``if'' at " << prev_else_pos << std::endl;
}

void error::excessive_indent(misc::pos_type const& pos)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    excessive indentation" << std::endl;
}

void error::flow_terminated(misc::pos_type const& this_pos, misc::pos_type const& prev_pos)
{
    _has_error = true;
    std::cerr << this_pos << std::endl;
    std::cerr << "    flow already terminated at " << prev_pos << std::endl;
}

void error::forbid_def_func(misc::pos_type const& pos, std::string const& name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    " << "attempt define function `" << name << "' but forbidden here." << std::endl;
}

void error::forbid_def_var(misc::pos_type const& pos, std::string const& name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    " << "attempt define variable `" << name << "' but forbidden here." << std::endl;
}

void error::var_already_in_local(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& var_name)
{
    _has_error = true;
    std::cerr << this_def_pos << std::endl;
    std::cerr << "    variable `" << var_name << "' already defined." << std::endl;
    std::cerr << "    see previous definition in local space at " << prev_def_pos << std::endl;
}

void error::var_ref_before_def(misc::pos_type const& def_pos
                             , std::list<misc::pos_type> const& ref_positions
                             , std::string const& name)
{
    _has_error = true;
    std::cerr << def_pos << std::endl;
    std::cerr << "    variable `" << name << "' definition after reference. see references at:" << std::endl;
    std::for_each(ref_positions.begin()
                , ref_positions.end()
                , [&](misc::pos_type const& pos)
                  {
                      std::cerr << "    - " << pos << std::endl;
                  });
}

void error::func_already_in_local(misc::pos_type const& prev_def_pos
                                , misc::pos_type const& this_def_pos
                                , std::string const& name
                                , int param_count)
{
    _has_error = true;
    std::cerr << this_def_pos << std::endl;
    std::cerr << "    function `" << name << "' with " << param_count << " parameter(s) already defined."
              << std::endl;
    std::cerr << "    see previous definition in local space at Line " << prev_def_pos << std::endl;
}

void error::func_shadow_external(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& name
                               , int param_count)
{
    _has_error = true;
    std::cerr << this_def_pos << std::endl;
    std::cerr << "    function `" << name << "' with " << param_count << " parameter(s) already defined."
              << std::endl;
    std::cerr << "    see previous definition in external space at " << prev_def_pos << std::endl;
}

void error::func_not_def(misc::pos_type const& ref_pos, std::string const& name, int param_count)
{
    _has_error = true;
    std::cerr << ref_pos << std::endl;
    std::cerr << "    function `" << name << "' with " << param_count << " parameter(s) not defined."
              << std::endl;
}

void error::var_not_def(misc::pos_type const& ref_pos, std::string const& name)
{
    _has_error = true;
    std::cerr << ref_pos << std::endl;
    std::cerr << "    variable `" << name << "' not defined in context." << std::endl;
}

void error::binary_op_not_avai(misc::pos_type const& pos
                             , std::string const& op_img
                             , std::string const& lhst_name
                             , std::string const& rhst_name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    no available binary operation " << op_img
              << " for type `" << lhst_name << "' and `" << rhst_name << "'." << std::endl;
}

void error::pre_unary_op_not_avai(misc::pos_type const& pos
                                , std::string const& op_img
                                , std::string const& rhst_name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    no available prefix unary operation " << op_img
              << " for type `" << rhst_name << "'." << std::endl;
}

void error::conflict_return_type(misc::pos_type const& this_pos
                               , std::string const& prev_ret_type_name
                               , std::string const& this_ret_type_name)
{
    _has_error = true;
    std::cerr << this_pos << std::endl;
    std::cerr << "    function return type conflict, previous return type: " << prev_ret_type_name << std::endl;
    std::cerr << "                                       this return type: " << this_ret_type_name << std::endl;
}

void error::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    _has_error = true;
    std::cerr << "Function return type is not resolvable:" << std::endl;
    std::cerr << "    name: `" << name << "' arg_count: " << arg_count << std::endl;
}

void error::cond_not_bool(misc::pos_type const& pos, std::string const& actual_type)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    condition type is not boolean, actual type: " << actual_type << std::endl;
}

void error::request_variable_not_callable(misc::pos_type const& call_pos)
{
    _has_error = true;
    std::cerr << call_pos << std::endl;
    std::cerr << "    variable not callable" << std::endl;
}
