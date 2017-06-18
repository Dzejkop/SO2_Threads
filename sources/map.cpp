#include "map.hpp"

#include <algorithm>

#include "config.hpp"
#include "stats.hpp"

bool Map::IsWall(ivec2 pos) const
{
    return !Get(pos) || Get(pos)->wall;
}

bool Map::IsOccupied(ivec2 pos) const
{
    m_mutex.lock();
    for (auto c : GetCars([](auto car) { return car->IsActive(); }))
    {
        if (c->GetPosition() == pos)
        {
            m_mutex.unlock();
            return true;
        }
    }

    m_mutex.unlock();
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

Car* Map::SpawnCar(ivec2 pos)
{
    m_mutex.lock();
    Car* c = new Car(*this, pos);

    m_cars.push_back(c);

    m_mutex.unlock();

    Statistics::Instance().LogCarSpawn();

    return c;
}

void Map::BuildGraph()
{
    auto allIntersections = GetSegments([](auto s) { return s->intersection; });

    for (auto intersection : allIntersections)
    {
        m_graph.NewNode(intersection->position);
    }

    for (auto intersection : allIntersections)
    {
        auto node = m_graph.Get(intersection->position);
        for (auto direction : GetIntersectionLanes(intersection->position))
        {
            ivec2 pCurrent = intersection->position;
            int cost = 0;

            do {pCurrent += direction; cost++; }
            while(!Get(pCurrent)->intersection);

            node->neighbours.push_back(std::make_pair(m_graph.Get(pCurrent), cost));
        }
    }
}

void Map::Loop()
{
    m_isActive = true;
    while (m_isActive)
    {
        auto deadCars = GetCars([](auto c) { return !c->IsActive(); });

        m_mutex.lock();

        for (auto car : deadCars)
        {
            m_cars.erase(std::find(m_cars.begin(), m_cars.end(), car));
        }

        m_mutex.unlock();

        if (m_cars.size() < Config::maxCarsInSimulation)
        {
            auto spawnPoints = GetSegments([](auto s) { return s->spawnPoint; });
            ivec2 randomPos = spawnPoints.at(rand() % spawnPoints.size())->position;
            SpawnCar(randomPos);
        }

        std::this_thread::sleep_for(Config::mapUpdateStep);
    }
}

void Map::Run()
{
    m_isActive = true;
    m_thread = std::thread(&Map::Loop, this);
}

void Map::Stop()
{
    m_isActive = false;
    m_thread.join();

    for (Car* c : m_cars)
    {
        c->Stop();
        delete c;
    }
}

void Map::GenerateMap(ivec2 size)
{
    for (int x = 0; x < size.x; x++)
    {
        m_map.push_back(std::vector<Segment>());

        for (int y = 0; y < size.y; y++)
        {
            m_map.at(x).push_back(Segment::Wall({x, y}));
        }
    }
}

void Map::LoadMap(const std::vector<std::string>& map)
{
    GenerateMap(ivec2(map.at(0).size(), map.size()));

    for (int y = 0; y < map.size(); y++)
    {
        for (int x = 0; x < map.at(y).size(); x++)
        {
            auto& mapRef = m_map.at(x).at(y);
            switch (map.at(y).at(x))
            {
                case '#':
                    mapRef = Segment::Intersection({x, y});
                    break;
                case '!':
                    mapRef = Segment::SpawnPoint({x, y});
                    break;
                case '@':
                    mapRef = Segment::DestinationPoint({x, y});
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
    m_mutex.lock();
}

void Map::UnlockMove() const
{
    m_mutex.unlock();
}

std::vector<const Map::Segment*> Map::GetSegments(Map::SegmentPredicate predicate) const
{
    std::vector<const Segment*> lst;
    for (const auto& row : m_map)
    {
        for (const auto& segment : row)
        {
            if (!predicate || (predicate && predicate(&segment)))
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

    return &(m_map.at(pos.x).at(pos.y));
}

std::vector<const Car*> Map::GetCars(CarPredicate predicate) const
{
    std::vector<const Car*> lst;
    for (auto car : m_cars)
    {
        if (!predicate || (predicate && predicate(car)))
        {
            lst.push_back(car);
        }
    }
    return lst;
}

ivec2 Map::Size() const
{
    return ivec2(m_map.size(), m_map.at(0).size());
}

Map::Segment Map::Segment::Wall(ivec2 pos)
{
    return Segment(pos, false, false, false, true, ivec2());
}

Map::Segment Map::Segment::Intersection(ivec2 pos)
{
    return Segment(pos, true, false, false, false, ivec2());
}

Map::Segment Map::Segment::Road(ivec2 pos, ivec2 dir)
{
    return Segment(pos, false, false, false, false, dir);
}

Map::Segment Map::Segment::SpawnPoint(ivec2 pos)
{
    return Segment(pos, true, true, false, false, ivec2());
}

Map::Segment Map::Segment::DestinationPoint(ivec2 pos)
{
    return Segment(pos, true, false, true, false, pos);
}

Map::Segment::Segment(ivec2 pos, bool i, bool sp, bool dp, bool w, ivec2 d)
    : position(pos)
    , intersection(i)
    , wall(w)
    , direction(d)
    , spawnPoint(sp)
    , destinationPoint(dp)
{
}
