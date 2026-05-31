#pragma once
#include <variant>
#include <string>
#include <cstdint>
#include <iostream>
#include <sstream>

#include "types.hpp"


template<typename... Ts>
struct Overload : Ts... { using Ts::operator()...; };

// primitive null value type
struct Null {};

// primitive database value types of a cell
using CellValue = std::variant<
    Null,
    int64_t, // 8 byte integer (big int)
    double,
    std::string,
    bool
>;

struct Cell {
    CellValue value;

    bool is_null() const { return std::holds_alternative<Null>(value); }
    
    std::string string() const {
        return std::visit(Overload{
            [](Null)                 -> std::string { return "null"; },
            [](int64_t v)            -> std::string { return std::to_string(v); },
            [](double v)             -> std::string { return std::to_string(v); },
            [](bool v)               -> std::string { return v ? "true" : "false"; },
            [](const std::string& v) -> std::string { return v; }
        }, value);
    }

    std::optional<DataType> type() const {
        return std::visit(Overload{
            [](Null)               -> std::optional<DataType> { return std::nullopt; },
            [](int64_t)            -> std::optional<DataType> { return DataType::Int64; },
            [](double)             -> std::optional<DataType> { return DataType::Double; },
            [](const std::string&) -> std::optional<DataType> { return DataType::String; },
            [](bool)               -> std::optional<DataType> { return DataType::Bool; }
        }, value);
    }
};
