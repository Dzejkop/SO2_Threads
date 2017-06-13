#include "common.hpp"

#include <ncurses.h>

using namespace std::literals::chrono_literals;

struct Car
{
    uvec2 position;
    uvec2 direction;
};

std::vector<std::string> MAP = {
"000##000",
"000##000",
"000##000",
"########",
"########",
"000##000",
"000##000",
"000##000"
};

std::mutex carsMutex;
std::vector<const Car*> cars;

const char* ROAD = " ";
const char* BUILDING = "#";

void carThread(uvec2 p, uvec2 d)
{
    Car car = {p, d};

    carsMutex.lock();
    cars.push_back(&car);
    carsMutex.unlock();

    while (true)
    {
        std::this_thread::sleep_for(30ms);

        car.position = car.position + car.direction;
    }
}

int columns = 0, rows = 0;

int main(int argc, char** argv)
{
    initscr();
    getmaxyx(stdscr, rows, columns);

    auto t1 = std::thread(carThread, uvec2(0,0), uvec2(0,1));

    while (true)
    {
        std::this_thread::sleep_for(20ms);

        for (uint y = 0; y < MAP.size(); y++)
        {
            for (uint x = 0; x < MAP[y].size(); x++)
            {
                mvprintw(x, y, MAP[y][x] != '#' ? "#" : " ");
            }
        }

        for (auto c : cars)
        {
            mvprintw(c->position.x, c->position.y, "@");
        }

        refresh();
    }

    endwin();

    return 0;
}