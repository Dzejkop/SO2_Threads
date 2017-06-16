#pragma once
#include "common.hpp"

#include "car.hpp"

#include "graph.hpp"

class Map
{
public:
    struct Segment
    {
        static Segment Wall(ivec2 pos);
        static Segment Intersection(ivec2 pos);
        static Segment Road(ivec2 pos, ivec2 dir);

        Segment(ivec2 pos, bool i, bool w, ivec2 d);

        ivec2 position;
        ivec2 direction;

        bool intersection;
        bool wall;
    };

    struct Section
    {
        const Segment* top;
        const Segment* bottom;
        const Segment* middle;
        const Segment* left;
        const Segment* right;
    };

    typedef bool (*SegmentPredicate)(const Segment*);

    void    LockMove()              const;
    void    UnlockMove()            const;
    bool    IsWall(ivec2 pos)       const;
    bool    IsOccupied(ivec2 pos)   const;
    Section GetSection(ivec2 pos)   const;
    std::vector<ivec2> GetIntersectionLanes(ivec2 pos) const;

    ivec2                       Size()                              const;
    const Segment*              Get(ivec2 pos)                      const;
    const std::vector<Car*>&    GetCars()                           const;
    std::vector<const Segment*> Filter(SegmentPredicate predicate)  const;

    void        Run();
    void        Stop();
    void        LoadMap(const std::vector<std::string>& map);
    void        GenerateMap(ivec2 size);
    const Car*  SpawnCar(ivec2 pos);

private:
    void BuildGraph();

    Graph                               _graph;
    std::vector<std::vector<Segment>>   _map;
    std::vector<Car*>                   _cars;
    std::vector<std::thread>            _carThreads;

    mutable std::mutex _mtx;
};
