#include <iostream>

#include "core/table.hpp"
#include "utils.hpp"


namespace {
    // internal table fixtures for test isolation — not exposed outside this file
    Column make_nullable_col()    { return Column{"dummy_string", DataType::String, true}; }
    Column make_non_nullable_col() { return Column{"dummy_int", DataType::Int64}; }
    Table  make_table()           { return Table{"dummy_table", {make_nullable_col(), make_non_nullable_col()}, {}}; }
}

void test_table(TestResults& results) {
    const std::string suite = "tests/cpp/unit/test_table.cpp";

    // nullable column
    run_test(suite, "nullable_column_name_correct", []() {
        std::string name = make_nullable_col().name;
        check(name == "dummy_string", "expected 'dummy_string' got '" + name + "'");
    }, results);
    run_test(suite, "nullable_column_data_type_correct", []() {
        check(make_nullable_col().type == DataType::String, "expected 'DataType::String'");
    }, results);
    run_test(suite, "nullable_column_is_nullable", []() {
        check(make_nullable_col().nullable == true, "expected nullable to be true");
    }, results);

    // nullable defaults to false
    run_test(suite, "column_nullable_defaults_to_false", []() {
        check(make_non_nullable_col().nullable == false, "expected nullable to default to false");
    }, results);

    // table
    run_test(suite, "table_name_correct", []() {
        std::string name = make_table().name;
        check(name == "dummy_table", "expected 'dummy_table' got '" + name + "'");
    }, results);
    run_test(suite, "table_column_count_correct", []() {
        int size = make_table().cols.size();
        check(size == 2, "expected 2 columns got " + std::to_string(size));
    }, results);
    run_test(suite, "table_row_count_zero_on_construction", []() {
        int size = make_table().rows.size();
        check(size == 0, "expected 0 rows got " + std::to_string(size));
    }, results);
}