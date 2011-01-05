#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "data-trees.h"
#include "../yy-misc.h"

using namespace test;

TEST(Syntax, Mix)
{
    yyparse();
    ASSERT_FALSE(parser::has_error());

    data_tree::expect_one()
        (misc::pos_type(1), 0, data_node_base::VAR_DEF, "n")
            (misc::pos_type(1), data_node_base::INTEGER, "10")

        (misc::pos_type(3), 0, data_node_base::FUNC_DEF_HEAD_BEGIN, "fib")
            (misc::pos_type(3), 0, data_node_base::IDENTIFIER, "n")
        (misc::pos_type(3), 0, data_node_base::FUNC_DEF_HEAD_END)

            (misc::pos_type(4), 1, data_node_base::FUNC_DEF_HEAD_BEGIN, "add")
            (misc::pos_type(4), 1, data_node_base::FUNC_DEF_HEAD_END)

                (misc::pos_type(5), 2, data_node_base::RETURN)
                    (misc::pos_type(5), data_node_base::BINARY_OP_BEGIN, "+")
                    (misc::pos_type(5), data_node_base::OPERAND)
                        (misc::pos_type(5), data_node_base::FUNC_CALL_BEGIN, "fib")
                        (misc::pos_type(5), data_node_base::ARGUMENT)
                            (misc::pos_type(5), data_node_base::IDENTIFIER, "n")
                        (misc::pos_type(5), data_node_base::FUNC_CALL_END)
                    (misc::pos_type(5), data_node_base::OPERAND)
                        (misc::pos_type(5), data_node_base::FUNC_CALL_BEGIN, "fib")
                        (misc::pos_type(5), data_node_base::ARGUMENT)
                            (misc::pos_type(5), data_node_base::BINARY_OP_BEGIN, "-")
                            (misc::pos_type(5), data_node_base::OPERAND)
                                (misc::pos_type(5), data_node_base::IDENTIFIER, "n")
                            (misc::pos_type(5), data_node_base::OPERAND)
                                (misc::pos_type(5), data_node_base::INTEGER, "1")
                            (misc::pos_type(5), data_node_base::BINARY_OP_END)
                        (misc::pos_type(5), data_node_base::FUNC_CALL_END)
                    (misc::pos_type(5), data_node_base::BINARY_OP_END)

            (misc::pos_type(6), 1, data_node_base::BRANCH_IFNOT)
            (misc::pos_type(6), 1, data_node_base::CONDITION_BEGIN)
                (misc::pos_type(6), data_node_base::BINARY_OP_BEGIN, "<")
                (misc::pos_type(6), data_node_base::OPERAND)
                    (misc::pos_type(6), data_node_base::IDENTIFIER, "n")
                (misc::pos_type(6), data_node_base::OPERAND)
                    (misc::pos_type(6), data_node_base::INTEGER, "2")
                (misc::pos_type(6), data_node_base::BINARY_OP_END)
            (misc::pos_type(6), 1, data_node_base::CONDITION_END)
                (misc::pos_type(7), 2, data_node_base::RETURN)
                        (misc::pos_type(7), data_node_base::FUNC_CALL_BEGIN, "add")
                        (misc::pos_type(7), data_node_base::FUNC_CALL_END)

            (misc::pos_type(8), 1, data_node_base::RETURN)
                (misc::pos_type(8), data_node_base::INTEGER, "1")

            (misc::pos_type(10), 0, data_node_base::BRANCH_IF)
            (misc::pos_type(10), 0, data_node_base::CONDITION_BEGIN)
                (misc::pos_type(10), data_node_base::BINARY_OP_BEGIN, ">")
                (misc::pos_type(10), data_node_base::OPERAND)
                    (misc::pos_type(10), data_node_base::IDENTIFIER, "n")
                (misc::pos_type(10), data_node_base::OPERAND)
                    (misc::pos_type(10), data_node_base::INTEGER, "5")
                (misc::pos_type(10), data_node_base::BINARY_OP_END)
            (misc::pos_type(10), 0, data_node_base::CONDITION_END)
                (misc::pos_type(11), 1, data_node_base::ARITHMETICS)
                    (misc::pos_type(11), data_node_base::FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(11), data_node_base::ARGUMENT)
                        (misc::pos_type(11), data_node_base::FUNC_CALL_BEGIN, "fib")
                            (misc::pos_type(11), data_node_base::ARGUMENT)
                                (misc::pos_type(11), data_node_base::INTEGER, "10")
                        (misc::pos_type(11), data_node_base::FUNC_CALL_END)
                    (misc::pos_type(11), data_node_base::FUNC_CALL_END)
            (misc::pos_type(12), 0, data_node_base::BRANCH_ELSE)
                (misc::pos_type(13), 1, data_node_base::ARITHMETICS)
                    (misc::pos_type(13), data_node_base::FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(13), data_node_base::ARGUMENT)
                            (misc::pos_type(13), data_node_base::BOOLEAN, "false")
                    (misc::pos_type(13), data_node_base::FUNC_CALL_END)

        (misc::pos_type(15), 0, data_node_base::FUNC_DEF_HEAD_BEGIN, "print_10")
        (misc::pos_type(15), 0, data_node_base::FUNC_DEF_HEAD_END)
            (misc::pos_type(16), 1, data_node_base::LOOP_WHILE)
            (misc::pos_type(16), 1, data_node_base::CONDITION_BEGIN)
                (misc::pos_type(16), data_node_base::BOOLEAN, "true")
            (misc::pos_type(16), 1, data_node_base::CONDITION_END)
                (misc::pos_type(17), 2, data_node_base::ARITHMETICS)
                    (misc::pos_type(17), data_node_base::FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(17), data_node_base::ARGUMENT)
                            (misc::pos_type(17), data_node_base::PRE_UNARY_OP_BEGIN, "+")
                            (misc::pos_type(17), data_node_base::OPERAND)
                                (misc::pos_type(17), data_node_base::FLOATING, "10.0")
                            (misc::pos_type(17), data_node_base::PRE_UNARY_OP_END)
                    (misc::pos_type(17), data_node_base::FUNC_CALL_END)
    ;
    data_tree::verify();
}
