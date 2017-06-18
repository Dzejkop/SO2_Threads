#include "config.hpp"

using namespace std::literals::chrono_literals;

const std::vector<std::chrono::milliseconds> Config::carUpdateDurations =
//{100ms, 150ms, 200ms, 250ms, 300ms};
// {60ms, 30ms, 80ms, 200ms, 150ms};
// {30ms, 40ms, 50ms, 60ms};
{200ms, 190ms, 180ms, 170ms, 160ms, 150ms, 140ms, 130ms, 100ms, 80ms, 60ms, 50ms, 50ms, 50ms, 50ms, 30ms};

const std::chrono::milliseconds Config::screenRefreshStep = 10ms;

const std::chrono::milliseconds Config::mapUpdateStep = 500ms;

const uint Config::maxCarsInSimulation = 20;
