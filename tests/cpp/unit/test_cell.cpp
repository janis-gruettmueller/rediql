#include <iostream>
#include <optional>

#include "core/cell.hpp"
#include "core/types.hpp"
#include "utils.hpp"


namespace {
    // internal cell factory for test isolation — not exposed outside this file
    template<typename T>
    Cell make_cell(T value) { return Cell{CellValue{std::in_place_type<T>, value}}; }
}

void test_cell(TestResults& results) {
    const std::string suite = "tests/cpp/unit/test_cell.cpp";

    // int64_t
    run_test(suite, "int64_t_cell_holds_correct_value_type", []() {
        check(std::holds_alternative<int64_t>(make_cell(int64_t{42}).value), "expected int64_t");
    }, results);
    run_test(suite, "int64_t_cell_prints_correctly", []() {
        std::string output = make_cell(int64_t{42}).string();
        check(output == "42", "expected '42' got '" + output + "'");
    }, results);
    run_test(suite, "int64_t_cell_data_type_correct", []() {
        std::optional<DataType> type = make_cell(int64_t{42}).type();
        check(type == DataType::Int64, "expected 'Int64' got '" + to_string(type.value()) + "'");
    }, results);
    run_test(suite, "int64_t_cell_returns_false_for_is_null_check", []() {
        check(make_cell(int64_t{42}).is_null() == false, "expected is_null() to return false");
    }, results);

    // double
    run_test(suite, "double_cell_holds_correct_value_type", []() {
        check(std::holds_alternative<double>(make_cell(double{41.1}).value), "expected double");
    }, results);
    run_test(suite, "double_cell_prints_correctly", []() {
        std::string output = make_cell(double{42.1}).string();
        check(output == "42.100000", "expected '42.100000' got '" + output + "'");
    }, results);
    run_test(suite, "double_cell_data_type_correct", []() {
        std::optional<DataType> type = make_cell(double{42.1}).type();
        check(type == DataType::Double, "expected 'Double' got '" + to_string(type.value()) + "'");
    }, results);
    run_test(suite, "double_cell_returns_false_for_is_null_check", []() {
        check(make_cell(double{42.1}).is_null() == false, "expected is_null() to return false");
    }, results);

    // std::string
    run_test(suite, "string_cell_holds_correct_value_type", []() {
        check(std::holds_alternative<std::string>(make_cell(std::string{"dummy"}).value), "expected std::string");
    }, results);
    run_test(suite, "string_cell_prints_correctly", []() {
        std::string output = make_cell(std::string{"dummy"}).string();
        check(output == "dummy", "expected 'dummy' got '" + output + "'");
    }, results);
    run_test(suite, "string_cell_data_type_correct", []() {
        std::optional<DataType> type = make_cell(std::string{"dummy"}).type();
        check(type == DataType::String, "expected 'String' got '" + to_string(type.value()) + "'");
    }, results);
    run_test(suite, "string_cell_returns_false_for_is_null_check", []() {
        check(make_cell(std::string{"dummy"}).is_null() == false, "expected is_null() to return false");
    }, results);

    // bool
    run_test(suite, "bool_cell_holds_correct_value_type", []() {
        check(std::holds_alternative<bool>(make_cell(bool{true}).value), "expected bool");
    }, results);
    run_test(suite, "bool_cell_prints_correctly", []() {
        std::string output = make_cell(bool{true}).string();
        check(output == "true", "expected 'true' got '" + output + "'");
    }, results);
    run_test(suite, "bool_cell_data_type_correct", []() {
        std::optional<DataType> type = make_cell(bool{true}).type();
        check(type == DataType::Bool, "expected 'Bool' got '" + to_string(type.value()) + "'");
    }, results);
    run_test(suite, "bool_cell_returns_false_for_is_null_check", []() {
        check(make_cell(bool{true}).is_null() == false, "expected is_null() to return false");
    }, results);

    // Null
    run_test(suite, "null_cell_holds_correct_value_type", []() {
        check(std::holds_alternative<Null>(make_cell(Null{}).value), "expected Null");
    }, results);
    run_test(suite, "null_cell_prints_correctly", []() {
        std::string output = make_cell(Null{}).string();
        check(output == "null", "expected 'null' got '" + output + "'");
    }, results);
    run_test(suite, "null_cell_returns_nullopt_correctly", []() {
        std::optional<DataType> type = make_cell(Null{}).type();
        check(type == std::nullopt, "expected std::nullopt");
    }, results);
    run_test(suite, "null_cell_returns_true_for_is_null_check", []() {
        check(make_cell(Null{}).is_null() == true, "expected is_null() to return true");
    }, results);
}