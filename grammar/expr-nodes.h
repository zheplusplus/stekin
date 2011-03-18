#ifndef __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__
#define __STAKCENING_GRAMMAR_EXPRESSION_NODES_H__

#include "../flowcheck/expr-nodes.h"

namespace grammar {

    typedef flchk::pre_unary_op pre_unary_op;
    typedef flchk::binary_op binary_op;
    typedef flchk::conjunction conjunction;
    typedef flchk::disjunction disjunction;
    typedef flchk::negation negation;
    typedef flchk::reference reference;
    typedef flchk::bool_literal bool_literal;
    typedef flchk::int_literal int_literal;
    typedef flchk::float_literal float_literal;
    typedef flchk::call call;

}

#endif /* _STAKCENING_GRAMMAR_EXPRESSION_NODES_H__ */
