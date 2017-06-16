#pragma once
#include "common.hpp"

class Config
{
public:
    const static std::vector<std::chrono::milliseconds> carUpdateDurations;
    const static std::chrono::milliseconds screenRefreshStep;
};
