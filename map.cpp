#include "map.hpp"

#include "config.hpp"

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

std::vector<ivec2> Map::GetIntersectionLanes(ivec2 pos) const
{
    std::vector<ivec2> lanes;

    Map::Section section = GetSection(pos);

    if (section.right && section.right->direction == ivec2(1, 0)) lanes.push_back(ivec2(1, 0));
    if (section.top && section.top->direction == ivec2(0, 1)) lanes.push_back(ivec2(0, 1));
    if (section.bottom && section.bottom->direction == ivec2(0, -1)) lanes.push_back(ivec2(0, -1));
    if (section.left && section.left->direction == ivec2(-1, 0)) lanes.push_back(ivec2(-1, 0));

    return lanes;
}

const Car* Map::SpawnCar(ivec2 pos)
{
    _mtx.lock();
    Car* c = new Car(*this, Config::carUpdateDurations.at(rand() % Config::carUpdateDurations.size()));

    c->SetPosition(pos);

    _cars.push_back(c);

    _mtx.unlock();

    return c;
}

const std::vector<Car*>& Map::GetCars() const
{
    return _cars;
}

void Map::BuildGraph()
{
    auto allIntersections = Filter([](auto s) { return s->intersection; });

    for (auto intersection : allIntersections)
    {
        _graph.NewNode(intersection->position);
    }

    for (auto intersection : allIntersections)
    {
        auto node = _graph.Get(intersection->position);
        for (auto direction : GetIntersectionLanes(intersection->position))
        {
            ivec2 pCurrent = intersection->position;
            int cost = 0;

            do {pCurrent += direction; cost++; }
            while(!Get(pCurrent)->intersection);

            node->neighbours.push_back(std::make_pair(_graph.Get(pCurrent), cost));
        }
    }
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
            _map.at(x).push_back(Segment::Wall({x, y}));
        }
    }
}

void Map::LoadMap(const std::vector<std::string>& map)
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
                    mapRef = Segment::Intersection({x, y});
                    break;
                case '>':
                    mapRef = Segment::Road({x, y}, {1, 0});
                    break;
                case '<':
                    mapRef = Segment::Road({x, y}, {-1, 0});
                    break;
                case '^':
                    mapRef = Segment::Road({x, y}, {0, -1});
                    break;
                case 'v':
                    mapRef = Segment::Road({x, y}, {0, 1});
                    break;
            }
        }
    }

    BuildGraph();
}

void Map::LockMove() const
{
    _mtx.lock();
}

void Map::UnlockMove() const
{
    _mtx.unlock();
}

std::vector<const Map::Segment*> Map::Filter(Map::SegmentPredicate predicate) const
{
    std::vector<const Segment*> lst;
    for (const auto& row : _map)
    {
        for (const auto& segment : row)
        {
            if (predicate(&segment))
            {
                lst.push_back(&segment);
            }
        }
    }
    return lst;
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

Map::Segment Map::Segment::Wall(ivec2 pos)
{
    return Segment(pos, false, true, ivec2());
}

Map::Segment Map::Segment::Intersection(ivec2 pos)
{
    return Segment(pos, true, false, ivec2());
}

Map::Segment Map::Segment::Road(ivec2 pos, ivec2 dir)
{
    return Segment(pos, false, false, dir);
}

Map::Segment::Segment(ivec2 pos, bool i, bool w, ivec2 d)
    : position(pos)
    , intersection(i)
    , wall(w)
    , direction(d)
{
}
