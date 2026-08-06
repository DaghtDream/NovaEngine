#pragma once
#include <iostream>
#include <chrono>
#include <string>

// ---------------- 高精度计时器 (ProfileBlock) ----------------
class ProfileBlock {
public:
    ProfileBlock(const std::string& name)
        : m_Name(name), m_Start(std::chrono::high_resolution_clock::now()) {
    }

    ~ProfileBlock() {
        auto end = std::chrono::high_resolution_clock::now();
        auto startUs = std::chrono::time_point_cast<std::chrono::microseconds>(m_Start).time_since_epoch().count();
        auto endUs = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch().count();
        auto durationUs = endUs - startUs;

        std::cout << "[PROFILE] " << m_Name << " -> "
            << durationUs << " us (" << durationUs / 1000.0f << " ms)\n";
    }

private:
    std::string m_Name;
    std::chrono::high_resolution_clock::time_point m_Start;
};