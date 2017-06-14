#include "common.hpp"

#include "map.hpp"
#include "car.hpp"

#include <ncurses.h>

using namespace std::literals::chrono_literals;

int columns = 0, rows = 0;

const std::chrono::milliseconds refreshStep = 30ms;

const std::vector<std::string> DEFAULT_MAP = {
    "............",
    ".##.#>#>>>#.",
    ".#..^ v...v.",
    "....^ v...v.",
    "....^ v...v.",
    "....^ v...v.",
    ".#>>#>#>>>#.",
    ".^..^.v...v.",
    ".#<<#<#<<<#.",
    "....^ v...v.",
    "....^ v<<<#.",
    "....^ v.....",
    ".#..^ v...#.",
    ".##.#<#..##.",
    "............"
};

std::vector<std::string> loadMapFromFile(std::string fname)
{
    return std::vector<std::string>();
}

int main(int argc, char** argv)
{
    initscr();
    printw("Starting...");

    printw("Press any key to start.");
    getch();

    curs_set(0);
    getmaxyx(stdscr, rows, columns);

    Map map;

    std::vector<std::string> map_str = DEFAULT_MAP;

    map.SpawnCar(ivec2(6, 7));
    map.SpawnCar(ivec2(1, 8));
    map.SpawnCar(ivec2(2, 8));
    map.SpawnCar(ivec2(4, 7));
    map.SpawnCar(ivec2(3, 8));
    map.SpawnCar(ivec2(4, 8));
    map.SpawnCar(ivec2(5, 8));
    map.SpawnCar(ivec2(6, 8));

    map.LoadMap(map_str);
    map.Run();

    start_color();
    init_pair( 1, COLOR_RED, COLOR_RED );
    init_pair( 2, COLOR_WHITE, COLOR_WHITE);

    while (true)
    {
        clear();

        attron(COLOR_PAIR(2));
        for (uint x = 0; x < map.Size().x; x++)
        {
            for (uint y = 0; y < map.Size().y; y++)
            {
                if (map.Get({x, y})->wall)
                    mvprintw(y, x, "#");
            }
        }
        attroff(COLOR_PAIR(2));

        attron(COLOR_PAIR(1));
        for (auto c : map.GetCars())
        {

            mvprintw(c->GetPosition().y, c->GetPosition().x, "O");

        }
        attroff(COLOR_PAIR(1));

        refresh();

        std::this_thread::sleep_for(refreshStep);
    }

    map.Stop();

    endwin();

    return 0;
}
