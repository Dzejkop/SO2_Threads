#include "car.hpp"
#include "map.hpp"

using namespace std::literals::chrono_literals;

const std::chrono::milliseconds carThreadStep = 60ms;

Car::Car(const Map& map)
    : _map(map)
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

        std::this_thread::sleep_for(carThreadStep);
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
    std::vector<ivec2> directions;

    Map::Section section = _map.GetSection(_position);

    if (section.right && section.right->direction == ivec2(1, 0)) directions.push_back(ivec2(1, 0));
    if (section.top && section.top->direction == ivec2(0, 1)) directions.push_back(ivec2(0, 1));
    if (section.bottom && section.bottom->direction == ivec2(0, -1)) directions.push_back(ivec2(0, -1));
    if (section.left && section.left->direction == ivec2(-1, 0)) directions.push_back(ivec2(-1, 0));

    return directions;
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
