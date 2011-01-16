#include <gtest/gtest.h>

#include "phony-err-report.h"
#include "test-common.h"
#include "../yy-misc.h"

using namespace test;

TEST(Syntax, Mix)
{
    yyparse();
    ASSERT_FALSE(parser::has_error());

    data_tree::expect_one()
        (misc::pos_type(1), 0, VAR_DEF, "n")
            (misc::pos_type(1), INTEGER, "10")

        (misc::pos_type(3), 0, FUNC_DEF_HEAD_BEGIN, "fib")
            (misc::pos_type(3), 0, IDENTIFIER, "n")
        (misc::pos_type(3), 0, FUNC_DEF_HEAD_END)

            (misc::pos_type(4), 1, FUNC_DEF_HEAD_BEGIN, "add")
            (misc::pos_type(4), 1, FUNC_DEF_HEAD_END)

                (misc::pos_type(5), 2, RETURN)
                    (misc::pos_type(5), BINARY_OP_BEGIN, "+")
                    (misc::pos_type(5), OPERAND)
                        (misc::pos_type(5), FUNC_CALL_BEGIN, "fib")
                        (misc::pos_type(5), ARGUMENT)
                            (misc::pos_type(5), IDENTIFIER, "n")
                        (misc::pos_type(5), FUNC_CALL_END)
                    (misc::pos_type(5), OPERAND)
                        (misc::pos_type(5), FUNC_CALL_BEGIN, "fib")
                        (misc::pos_type(5), ARGUMENT)
                            (misc::pos_type(5), BINARY_OP_BEGIN, "-")
                            (misc::pos_type(5), OPERAND)
                                (misc::pos_type(5), IDENTIFIER, "n")
                            (misc::pos_type(5), OPERAND)
                                (misc::pos_type(5), INTEGER, "1")
                            (misc::pos_type(5), BINARY_OP_END)
                        (misc::pos_type(5), FUNC_CALL_END)
                    (misc::pos_type(5), BINARY_OP_END)

            (misc::pos_type(6), 1, BRANCH_IFNOT)
            (misc::pos_type(6), 1, CONDITION_BEGIN)
                (misc::pos_type(6), BINARY_OP_BEGIN, "<")
                (misc::pos_type(6), OPERAND)
                    (misc::pos_type(6), IDENTIFIER, "n")
                (misc::pos_type(6), OPERAND)
                    (misc::pos_type(6), INTEGER, "2")
                (misc::pos_type(6), BINARY_OP_END)
            (misc::pos_type(6), 1, CONDITION_END)
                (misc::pos_type(7), 2, RETURN)
                        (misc::pos_type(7), FUNC_CALL_BEGIN, "add")
                        (misc::pos_type(7), FUNC_CALL_END)

            (misc::pos_type(8), 1, RETURN)
                (misc::pos_type(8), INTEGER, "1")

            (misc::pos_type(10), 0, BRANCH_IF)
            (misc::pos_type(10), 0, CONDITION_BEGIN)
                (misc::pos_type(10), BINARY_OP_BEGIN, ">")
                (misc::pos_type(10), OPERAND)
                    (misc::pos_type(10), IDENTIFIER, "n")
                (misc::pos_type(10), OPERAND)
                    (misc::pos_type(10), INTEGER, "5")
                (misc::pos_type(10), BINARY_OP_END)
            (misc::pos_type(10), 0, CONDITION_END)
                (misc::pos_type(11), 1, ARITHMETICS)
                    (misc::pos_type(11), FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(11), ARGUMENT)
                        (misc::pos_type(11), FUNC_CALL_BEGIN, "fib")
                            (misc::pos_type(11), ARGUMENT)
                                (misc::pos_type(11), INTEGER, "10")
                        (misc::pos_type(11), FUNC_CALL_END)
                    (misc::pos_type(11), FUNC_CALL_END)
            (misc::pos_type(12), 0, BRANCH_ELSE)
                (misc::pos_type(13), 1, ARITHMETICS)
                    (misc::pos_type(13), FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(13), ARGUMENT)
                            (misc::pos_type(13), BOOLEAN, "false")
                    (misc::pos_type(13), FUNC_CALL_END)

        (misc::pos_type(15), 0, FUNC_DEF_HEAD_BEGIN, "print_10")
        (misc::pos_type(15), 0, FUNC_DEF_HEAD_END)
            (misc::pos_type(16), 1, LOOP_WHILE)
            (misc::pos_type(16), 1, CONDITION_BEGIN)
                (misc::pos_type(16), BOOLEAN, "true")
            (misc::pos_type(16), 1, CONDITION_END)
                (misc::pos_type(17), 2, ARITHMETICS)
                    (misc::pos_type(17), FUNC_CALL_BEGIN, "write")
                        (misc::pos_type(17), ARGUMENT)
                            (misc::pos_type(17), PRE_UNARY_OP_BEGIN, "+")
                            (misc::pos_type(17), OPERAND)
                                (misc::pos_type(17), FLOATING, "10.0")
                            (misc::pos_type(17), PRE_UNARY_OP_END)
                    (misc::pos_type(17), FUNC_CALL_END)
    ;
    data_tree::verify();
}
