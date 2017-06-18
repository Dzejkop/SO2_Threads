#include "stats.hpp"

Statistics& Statistics::Instance()
{
    static Statistics instance;
    return instance;
}

Statistics::Statistics()
    : m_averageTravelTime(.0f)
    , m_averageTravelTimeLogCount(0)
    , m_startTime(std::chrono::system_clock::now())
{
}

float Statistics::GetAverageTravelTimeInSeconds() const
{
    return m_averageTravelTime;
}

float Statistics::GetSimulationRunningTimeInSeconds() const
{
    std::chrono::duration<float> d = std::chrono::system_clock::now() - m_startTime;
    return d.count();
}

int Statistics::GetTotalCarsSpawnedCount() const
{
    return m_totalSpawnedCars;
}

void Statistics::LogTravelTimeIn(float seconds)
{
    m_mutex.lock();

    m_averageTravelTimeLogCount++;

    m_averageTravelTime = ((m_averageTravelTime * (m_averageTravelTimeLogCount - 1)) + seconds) / (float) m_averageTravelTimeLogCount;

    m_mutex.unlock();
}

void Statistics::LogCarSpawn()
{
    m_mutex.lock();

    m_totalSpawnedCars++;

    m_mutex.unlock();
}
