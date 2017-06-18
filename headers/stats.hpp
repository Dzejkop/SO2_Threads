#pragma once
#include "common.hpp"

class Statistics
{
public:
    static Statistics& Instance();

    Statistics();

    float GetAverageTravelTimeInSeconds() const;
    float GetSimulationRunningTimeInSeconds() const;
    int GetTotalCarsSpawnedCount() const;

    void LogTravelTimeIn(float seconds);
    void LogCarSpawn();

private:
    mutable std::mutex m_mutex;

    float m_averageTravelTime;
    uint m_averageTravelTimeLogCount;
    uint m_totalSpawnedCars;

    std::chrono::time_point<std::chrono::system_clock> m_startTime;
};
