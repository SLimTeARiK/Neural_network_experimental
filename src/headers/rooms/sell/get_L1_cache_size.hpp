#include <iostream>
#include <string>
#include <cstdint>

#if defined(_WIN32)
#include <windows.h>
#include <vector>
#elif defined(__linux__)
#include <fstream>
#include <sstream>
#include <regex>
#endif

// Получение размера L1 кэша (в байтах)
uint64_t GetL1CacheSize() {
    uint64_t l1CacheSize = 0;

    // Windows
    #if defined(_WIN32)
    DWORD bufferSize = 0;
    GetLogicalProcessorInformation(nullptr, &bufferSize);
    std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
    if (GetLogicalProcessorInformation(buffer.data(), &bufferSize)) {
        for (const auto& info : buffer) {
            if (info.Relationship == RelationCache && info.Cache.Level == 1) {
                l1CacheSize = info.Cache.Size;
                break;
            }
        }
    }

    // Linux (через /sys/devices/system/cpu/cpu0/cache/index0/size)
    #elif defined(__linux__)
    std::ifstream cacheInfo("/sys/devices/system/cpu/cpu0/cache/index0/size");
    if (cacheInfo.is_open()) {
        std::string line;
        if (std::getline(cacheInfo, line)) {
            // Пример строки: "32K" → 32 * 1024
            std::regex sizeRegex(R"((\d+)([KkMmGg]?))");
            std::smatch match;
            if (std::regex_match(line, match, sizeRegex)) {
                uint64_t size = std::stoull(match[1].str());
                char unit = match[2].str().empty() ? ' ' : std::tolower(match[2].str()[0]);
                switch (unit) {
                    case 'k': l1CacheSize = size * 1024; break;
                    case 'm': l1CacheSize = size * 1024 * 1024; break;
                    case 'g': l1CacheSize = size * 1024 * 1024 * 1024; break;
                    default:  l1CacheSize = size; break;
                }
            }
        }
    }
    #endif

    return l1CacheSize;
}

