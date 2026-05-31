#include <iostream>
#include <chrono>

#include "utils.hpp"


void test_cell(TestResults& results);
void test_table(TestResults& results);
void print_summary(const TestResults& results, std::chrono::high_resolution_clock::time_point start);
void print_header();

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    TestResults results;
    print_header();

    test_cell(results);
    test_table(results);

    print_summary(results, start);
    return 0;
}