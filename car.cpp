#include "car.hpp"
#include "map.hpp"

using namespace std::literals::chrono_literals;

Car::Car(const Map& map, std::chrono::milliseconds stepTime)
    : _map(map)
    , _updateTime(stepTime)
{
}

void Car::Run()
{
    while (true)
    {
        auto currentSegment = _map.Get(_position);

        if (!currentSegment->intersection)
        {
            Move();
        }
        else
        {
            std::vector<ivec2> possibleDirections = GetPossibleDirections();
            ivec2 chosenDir = possibleDirections.at(std::rand() % possibleDirections.size());

            _direction = chosenDir;
            Move();
        }

        std::this_thread::sleep_for(_updateTime);
    }
}

void Car::SetPosition(ivec2 pos)
{
    _position = pos;
}

void Car::Move()
{
    _mutex.lock();
    if (!_map.Get(_position)->intersection)
        _direction = _map.Get(_position)->direction;
    _mutex.unlock();

    while(IsNextOccupied())
    {
        std::this_thread::sleep_for(500ms);
    }

    _mutex.lock();
    _position = _position + _direction;
    _mutex.unlock();
}

std::vector<ivec2> Car::GetPossibleDirections() const
{
    return _map.GetIntersectionLanes(_position);
}

const ivec2& Car::GetPosition() const
{
    return _position;
}

const ivec2 &Car::GetDirection() const
{
    return _direction;
}

ivec2 Car::GetNext() const
{
    return _position + _direction;
}

bool Car::IsNextOccupied() const
{
    return _map.IsOccupied(GetNext());
}
