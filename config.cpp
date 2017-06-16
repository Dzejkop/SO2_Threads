#include "config.hpp"

using namespace std::literals::chrono_literals;

const std::vector<std::chrono::milliseconds> Config::carUpdateDurations =
{100ms, 150ms, 200ms, 250ms, 300ms};
// {60ms, 30ms, 80ms, 200ms, 150ms};

const std::chrono::milliseconds Config::screenRefreshStep = 100ms;
