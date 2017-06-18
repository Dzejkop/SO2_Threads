#include "car.hpp"
#include "map.hpp"

#include "config.hpp"
#include "stats.hpp"

Car::Car(const Map& map)
    : m_map(map)
    , m_thread(&Car::Run, this)
    , m_speedLevel(0)
    , m_emergencyStop(false)
{
    auto intersectinos = m_map.GetSegments([](auto s) { return s->destinationPoint; });
    m_targetPosition = intersectinos.at(rand() % intersectinos.size())->position;
}

Car::Car(const Map &map, const ivec2 &pos)
    : m_map(map)
    , m_position(pos)
    , m_thread(&Car::Run, this)
    , m_speedLevel(0)
    , m_emergencyStop(false)
{
    auto intersectinos = m_map.GetSegments([](auto s) { return s->destinationPoint; });
    m_targetPosition = intersectinos.at(rand() % intersectinos.size())->position;
}

Car::~Car()
{
    m_thread.join();
}

void Car::Run()
{
    auto spawnTime = std::chrono::system_clock::now();

    while (IsActive())
    {
        auto currentSegment = m_map.Get(m_position);

        if (!currentSegment->intersection)
        {
            Move();
        }
        else
        {
            m_speedLevel = Config::carUpdateDurations.size() / 2;
            std::vector<ivec2> possibleDirections = GetPossibleDirections();
            ivec2 chosenDir = possibleDirections.at(std::rand() % possibleDirections.size());

            m_direction = chosenDir;
            Move();
        }

        m_speedLevel++;
        std::this_thread::sleep_for(ResolveSpeed());
    }

    std::chrono::duration<float> travelTime = std::chrono::system_clock::now() - spawnTime;
    Statistics::Instance().LogTravelTimeIn(travelTime.count());
}

void Car::SetPosition(ivec2 pos)
{
    m_position = pos;
}

void Car::Stop()
{
    m_emergencyStop = true;
}

void Car::Move()
{
    m_mutex.lock();
    if (!m_map.Get(m_position)->intersection)
        m_direction = m_map.Get(m_position)->direction;
    m_mutex.unlock();

    while(IsNextOccupied())
    {
        m_speedLevel = 0;
        std::this_thread::sleep_for(ResolveSpeed());
    }

    m_mutex.lock();
    m_position = m_position + m_direction;
    m_mutex.unlock();
}

std::vector<ivec2> Car::GetPossibleDirections() const
{
    return m_map.GetIntersectionLanes(m_position);
}

std::chrono::milliseconds Car::ResolveSpeed() const
{
    if (m_speedLevel >= Config::carUpdateDurations.size())
        return Config::carUpdateDurations.back();

    return Config::carUpdateDurations.at(m_speedLevel);
}

const ivec2& Car::GetPosition() const
{
    return m_position;
}

const ivec2 &Car::GetDirection() const
{
    return m_direction;
}

ivec2 Car::GetNext() const
{
    return m_position + m_direction;
}

bool Car::IsActive() const
{
    return !m_emergencyStop && (m_position != m_targetPosition);
}

bool Car::IsNextOccupied() const
{
    return m_map.IsOccupied(GetNext());
}
