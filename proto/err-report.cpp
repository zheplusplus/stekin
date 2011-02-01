#include <iostream>
#include <algorithm>

#include "err-report.h"

static bool _has_error = false;

bool proto::has_error()
{
    return _has_error;
}

void proto::forbid_def_var(misc::pos_type const& pos, std::string const& name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    " << "attempt define variable `" << name << "' but forbidden here." << std::endl;
}

void proto::forbid_def_func(misc::pos_type const& pos, std::string const& name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    " << "attempt define function `" << name << "' but forbidden here." << std::endl;
}

void proto::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    _has_error = true;
    std::cerr << "Function return type is not resolvable:" << std::endl;
    std::cerr << "    name: `" << name << "' arg_count: " << arg_count << std::endl;
}

void proto::var_already_in_local(misc::pos_type const& prev_def_pos
                               , misc::pos_type const& this_def_pos
                               , std::string const& var_name)
{
    _has_error = true;
    std::cerr << this_def_pos << std::endl;
    std::cerr << "    variable `" << var_name << "' already defined." << std::endl;
    std::cerr << "    see previous definition in local space at " << prev_def_pos << std::endl;
}

void proto::var_ref_before_def(misc::pos_type const& def_pos
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

void proto::func_already_in_local(misc::pos_type const& prev_def_pos
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

void proto::func_shadow_external(misc::pos_type const& prev_def_pos
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

void proto::func_not_def(misc::pos_type const& ref_pos, std::string const& name, int param_count)
{
    _has_error = true;
    std::cerr << ref_pos << std::endl;
    std::cerr << "    function `" << name << "' with " << param_count << " parameter(s) not defined."
              << std::endl;
}
