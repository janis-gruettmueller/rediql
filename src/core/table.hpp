#pragma once
#include <string>
#include <vector>

#include "cell.hpp"
#include "types.hpp"


struct Column {
    std::string name;
    DataType type;
    bool nullable = false;
};

struct Table {
    std::string name;
    std::vector<Column> cols;
    std::vector<std::vector<CellValue>> rows;
};