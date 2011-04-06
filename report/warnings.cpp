#include <iostream>

#include "warnings.h"

void warning::one_or_two_branches_terminated(
        util::sref<misc::pos_type const> consq_term_pos_or_null_if_not_term
      , util::sref<misc::pos_type const> alter_term_pos_or_null_if_not_term)
{
    std::cerr << "Warning: branch termination panic." << std::endl;
    if (bool(consq_term_pos_or_null_if_not_term)) {
        std::cerr << "    termination at: " << consq_term_pos_or_null_if_not_term->str() << std::endl;
    }
    if (bool(alter_term_pos_or_null_if_not_term)) {
        std::cerr << "    termination at: " << alter_term_pos_or_null_if_not_term->str() << std::endl;
    }
}
