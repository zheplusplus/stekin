#include <iostream>
#include <algorithm>

#include "err-report.h"

static bool _has_error = false;

bool inst::has_error()
{
    return _has_error;
}

void inst::var_not_def(misc::pos_type const& ref_pos, std::string const& name)
{
    _has_error = true;
    std::cerr << ref_pos << std::endl;
    std::cerr << "    variable `" << name << "' not defined in context." << std::endl;
}

void inst::binary_op_not_avai(misc::pos_type const& pos
                            , std::string const& op_img
                            , std::string const& lhst_name
                            , std::string const& rhst_name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    no available binary operation " << op_img
              << " for type `" << lhst_name << "' and `" << rhst_name << "'." << std::endl;
}

void inst::pre_unary_op_not_avai(misc::pos_type const& pos
                               , std::string const& op_img
                               , std::string const& rhst_name)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    no available prefix unary operation " << op_img
              << " for type `" << rhst_name << "'." << std::endl;
}

void inst::conflict_return_type(misc::pos_type const& this_pos
                              , std::string const& prev_ret_type_name
                              , std::string const& this_ret_type_name)
{
    _has_error = true;
    std::cerr << this_pos << std::endl;
    std::cerr << "    function return type conflict, previous return type: " << prev_ret_type_name << std::endl;
    std::cerr << "                                       this return type: " << this_ret_type_name << std::endl;
}

void inst::func_ret_type_unresolvable(std::string const& name, int arg_count)
{
    _has_error = true;
    std::cerr << "Function return type is not resolvable:" << std::endl;
    std::cerr << "    name: `" << name << "' arg_count: " << arg_count << std::endl;
}

void inst::cond_not_bool(misc::pos_type const& pos, std::string const& actual_type)
{
    _has_error = true;
    std::cerr << pos << std::endl;
    std::cerr << "    condition type is not boolean, actual type: " << actual_type << std::endl;
}
