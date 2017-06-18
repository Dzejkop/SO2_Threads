#pragma once
#include "common.hpp"

class Map;

class Car
{
public:
    Car(const Map& map);
    Car(const Map& map, const ivec2& pos);
    ~Car();

    const ivec2&    GetPosition()   const;
    const ivec2&    GetDirection()  const;
    ivec2           GetNext()       const;

    bool IsActive()                 const;
    bool IsNextOccupied()           const;
    bool IsAtIntersection()         const;
    void SetPosition(ivec2 pos);

    void Stop();

private:
    void Run();
    void Move();

    std::vector<ivec2>          GetPossibleDirections() const;
    std::chrono::milliseconds   ResolveSpeed()          const;

    ivec2                       m_position;
    ivec2                       m_direction;
    ivec2                       m_targetPosition;
    const Map&                  m_map;
    uint                        m_speedLevel;
    std::chrono::milliseconds   m_updateTime;
    std::thread                 m_thread;
    mutable std::mutex          m_mutex;

    bool                        m_emergencyStop;
};
