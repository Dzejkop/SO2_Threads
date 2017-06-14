#pragma once
#include "common.hpp"

#include "car.hpp"

class Map
{
public:
    struct Segment
    {
        static Segment Wall();
        static Segment Intersection();
        static Segment Road(ivec2 dir);

        Segment(bool i, bool w, ivec2 d);

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

    bool IsWall(ivec2 pos) const;
    bool IsOccupied(ivec2 pos) const;
    Section GetSection(ivec2 pos) const;

    const Car* SpawnCar(ivec2 pos);

    void Run();
    void Stop();
    void LoadMap(std::vector<std::string> map);
    void GenerateMap(ivec2 size);

    void LockMove() const;
    void UnlockMove() const;

    const Segment* Get(ivec2 pos) const;
    ivec2 Size() const;

    const std::vector<Car*>& GetCars() const;

private:
    std::vector<std::vector<Segment>> _map;
    std::vector<Car*> _cars;

    std::vector<std::thread> _carThreads;

    mutable std::mutex _mtx;
};
