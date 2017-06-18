#pragma once
#include "common.hpp"

#include "car.hpp"
#include "graph.hpp"

class Map
{
public:
    class Segment
    {
    public:
        Segment(ivec2 pos, bool i, bool sp, bool dp, bool w, ivec2 d);

        static Segment DestinationPoint(ivec2 pos);
        static Segment Road(ivec2 pos, ivec2 dir);
        static Segment Intersection(ivec2 pos);
        static Segment SpawnPoint(ivec2 pos);
        static Segment Wall(ivec2 pos);

        bool    destinationPoint;
        bool    intersection;
        bool    spawnPoint;
        ivec2   direction;
        ivec2   position;
        bool    wall;
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
    typedef bool (*CarPredicate)    (const Car*);

    std::vector<ivec2>          GetIntersectionLanes(ivec2 pos)                     const;
    Section                     GetSection(ivec2 pos)                               const;
    bool                        IsOccupied(ivec2 pos)                               const;
    bool                        IsSimulationActive()                                const;
    bool                        IsWall(ivec2 pos)                                   const;
    void                        UnlockMove()                                        const;
    void                        LockMove()                                          const;

    std::vector<const Segment*> GetSegments(SegmentPredicate predicate = nullptr)   const;
    std::vector<const Car*>     GetCars(CarPredicate predicate = nullptr)           const;
    const Segment*              Get(ivec2 pos)                                      const;
    ivec2                       Size()                                              const;

    void        LoadMap(const std::vector<std::string>& map);
    void        GenerateMap(ivec2 size);
    Car*        SpawnCar(ivec2 pos);
    void        Stop();
    void        Run();

private:
    void BuildGraph();
    void Loop();

    bool m_isActive;

    std::vector<std::vector<Segment>>   m_map;
    std::vector<Car*>                   m_cars;
    std::thread                         m_thread;
    Graph                               m_graph;

    mutable std::mutex m_mutex;
};
