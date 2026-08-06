#pragma once
#include <iostream>
#include <chrono>
#include <string>



// ---------------- ∂œ—‘”Î≤‚ ‘Õ≥º∆∫Í ----------------
inline int g_PassCount = 0;
inline int g_FailCount = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            std::cout << "  [PASS] " << message << "\n"; \
            g_PassCount++; \
        } else { \
            std::cerr << "  [FAIL] " << message << " (Line " << __LINE__ << ")\n"; \
            g_FailCount++; \
        } \
    } while(0)

inline void PrintTestSummary() {
    std::cout << "\n========================================\n";
    std::cout << " TEST SUMMARY: " << g_PassCount << " Passed, " << g_FailCount << " Failed.\n";
    std::cout << "========================================\n";
}