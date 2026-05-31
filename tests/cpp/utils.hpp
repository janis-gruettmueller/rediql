#pragma once
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>


// compiler detection
#if defined(__clang__)
    #define REDIQL_COMPILER "clang++ " + std::string(__clang_version__)
#elif defined(__GNUC__)
    #define REDIQL_COMPILER "g++ " + std::string(__VERSION__)
#elif defined(_MSC_VER)
    #define REDIQL_COMPILER "msvc " + std::to_string(_MSC_VER)
#else
    #define REDIQL_COMPILER "unknown"
#endif

// platform detection
#if defined(__APPLE__)
    #define REDIQL_PLATFORM "darwin"
#elif defined(__linux__)
    #define REDIQL_PLATFORM "linux"
#elif defined(_WIN32)
    #define REDIQL_PLATFORM "windows"
#else
    #define REDIQL_PLATFORM "unknown"
#endif


struct TestResults {
    int passed = 0;
    int failed = 0;
    std::vector<std::pair<std::string, std::string>> failures; // {suite::name, message}
};

inline void print_header() {
    std::cout << "\n========== RediQL Test Suite ==========" << std::endl;
    std::cout << "platform: " << REDIQL_PLATFORM 
              << " | compiler: " << REDIQL_COMPILER
              << " | standard: C++23" << std::endl;
    std::cout << "rootdir: " << std::filesystem::current_path().string() << std::endl;
    std::cout << std::endl;
}

inline void check(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

inline void run_test(const std::string& suite, const std::string& name, std::function<void()> test, TestResults& results) {
    try {
        test();
        results.passed++;
        std::cout << suite << "::" << name << " PASSED" << std::endl;
    } catch (const std::exception& e) {
        results.failed++;
        results.failures.push_back({suite + "::" + name, e.what()});
        std::cout << suite << "::" << name << " FAILED" << std::endl;
    }
}

inline void print_summary(const TestResults& results, std::chrono::high_resolution_clock::time_point start) {
    auto end = std::chrono::high_resolution_clock::now();
    auto s = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() * 1000;

    if (!results.failures.empty()) {
        std::cout << "\nFAILED TESTS" << std::endl;
        for (const auto& [name, message] : results.failures) {
            std::cout << "  " << name << std::endl;
            std::cout << "    " << message << std::endl;
        }
    }
    std::cout << "\n========== " << results.passed << " passed";
    if (results.failed > 0) {
        std::cout << ", " << results.failed << " failed";
    }
    std::cout << " in " << s << "s ==========" << std::endl;
}