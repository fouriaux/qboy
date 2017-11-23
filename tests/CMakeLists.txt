add_test(NAME integration_test_01 COMMAND empty_args.sh)
set_tests_properties(integration_test_01
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_02 COMMAND do.sh)
set_tests_properties(integration_test_02
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_03 COMMAND dont_do.sh)
set_tests_properties(integration_test_03
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_04 COMMAND go.sh)
set_tests_properties(integration_test_04
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_05 COMMAND \?.sh)
set_tests_properties(integration_test_05
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_06 COMMAND list.sh)
set_tests_properties(integration_test_06
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_07 COMMAND stop.sh)
set_tests_properties(integration_test_07
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

add_test(NAME integration_test_08 COMMAND not_existing_arg.sh)
set_tests_properties(integration_test_08
    PROPERTIES PASS_REGULAR_EXPRESSION "qboy [")

