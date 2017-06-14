#include "map.hpp"

bool Map::IsWall(ivec2 pos) const
{
    return !Get(pos) || Get(pos)->wall;
}

bool Map::IsOccupied(ivec2 pos) const
{
    _mtx.lock();
    for (Car* c : _cars)
    {
        if (c->GetPosition() == pos)
        {
            _mtx.unlock();
            return true;
        }
    }

    _mtx.unlock();
    return false;
}

Map::Section Map::GetSection(ivec2 pos) const
{
    Section s;

    s.top = Get(pos + ivec2(0, 1)); // top
    s.bottom = Get(pos + ivec2(0, -1)); // bottom
    s.middle = Get(pos); // middle
    s.left = Get(pos + ivec2(-1, 0)); // left
    s.right = Get(pos + ivec2(1, 0)); // right

    return s;
}

const Car* Map::SpawnCar(ivec2 pos)
{
    _mtx.lock();
    Car* c = new Car(*this);

    c->SetPosition(pos);

    _cars.push_back(c);

    _mtx.unlock();

    return c;
}

const std::vector<Car*>& Map::GetCars() const
{
    return _cars;
}

void Map::Run()
{
    for (Car* c : _cars)
    {
        _carThreads.push_back(std::thread(&Car::Run, c));
    }
}

void Map::Stop()
{
    for (std::thread& t : _carThreads)
    {
        t.join();
    }
}

void Map::GenerateMap(ivec2 size)
{
    for (uint x = 0; x < size.x; x++)
    {
        _map.push_back(std::vector<Segment>());

        for (uint y = 0; y < size.y; y++)
        {
            _map.at(x).push_back(Segment::Wall());
        }
    }
}

void Map::LoadMap(std::vector<std::string> map)
{
    GenerateMap(ivec2(map.at(0).size(), map.size()));

    for (uint y = 0; y < map.size(); y++)
    {
        for (uint x = 0; x < map.at(y).size(); x++)
        {
            auto& mapRef = _map.at(x).at(y);
            switch (map.at(y).at(x))
            {
                case '#':
                    mapRef = Segment::Intersection();
                    break;
                case '>':
                    mapRef = Segment::Road({1, 0});
                    break;
                case '<':
                    mapRef = Segment::Road({-1, 0});
                    break;
                case '^':
                    mapRef = Segment::Road({0, -1});
                    break;
                case 'v':
                    mapRef = Segment::Road({0, 1});
                    break;
            }
        }
    }
}

void Map::LockMove() const
{
    _mtx.lock();
}

void Map::UnlockMove() const
{
    _mtx.unlock();
}

const Map::Segment* Map::Get(ivec2 pos) const
{
    if (pos.x < 0 || pos.x >= Size().x || pos.y < 0 || pos.y >= Size().y)
            return nullptr;

    return &(_map.at(pos.x).at(pos.y));
}

ivec2 Map::Size() const
{
    return ivec2(_map.size(), _map.at(0).size());
}

Map::Segment Map::Segment::Wall()
{
    return Segment(false, true, ivec2());
}

Map::Segment Map::Segment::Intersection()
{
    return Segment(true, false, ivec2());
}

Map::Segment Map::Segment::Road(ivec2 dir)
{
    return Segment(false, false, dir);
}

Map::Segment::Segment(bool i, bool w, ivec2 d)
    : intersection(i)
    , wall(w)
    , direction(d)
{
}
