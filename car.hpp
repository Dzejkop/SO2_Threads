#pragma once
#include "common.hpp"

class Map;

class Car
{
public:
    Car(const Map& map);

    void Run();

    const ivec2& GetPosition() const;
    const ivec2& GetDirection() const;

    ivec2 GetNext() const;

    bool IsNextOccupied() const;
    bool IsAtIntersection() const;
    void SetPosition(ivec2 pos);

private:
    void Move();

    std::vector<ivec2> GetPossibleDirections() const;

    ivec2 _position;
    ivec2 _direction;

    const Map& _map;

    mutable std::mutex _mutex;
};
