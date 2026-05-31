#pragma once
#include <string>

enum class DataType { 
    Int64,
    Double,
    String,
    Bool
};

inline std::string to_string(DataType type) { 
    switch (type) {
        case DataType::Int64:  return "int64";
        case DataType::Double: return "double";
        case DataType::String: return "string";
        case DataType::Bool:   return "bool";
    }
}