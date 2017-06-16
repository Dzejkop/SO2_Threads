#include "common.hpp"

#include "map.hpp"
#include "car.hpp"

#include <ncurses.h>

#include <fstream>

#include "graph.hpp"
#include "config.hpp"

using namespace std::literals::chrono_literals;

int columns = 0, rows = 0;

const std::vector<std::string> DEFAULT_MAP = {
    "............",
    "....#>#>>>#.",
    "....^ v...v.",
    "....^ v...v.",
    "....^ v...v.",
    "....^ v...v.",
    ".#>>#>#>>>#.",
    ".^..^.v...v.",
    ".#<<#<#<<<#.",
    "....^ v...v.",
    "....^ #<<<#.",
    "....^ v.....",
    "....^ v.....",
    "....#<#.....",
    "............"
};

std::vector<std::string> loadMapFromFile(std::string fname)
{
    std::ifstream fileIn(fname);

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(fileIn, line))
    {
        lines.push_back(line);
    }

    return lines;
}

int main(int argc, char** argv)
{
    initscr();
    printw("Starting...\n");

    Map map;

    if (argc >= 2)
    {
        std::string mapFile(argv[1]);
        printw("Loading custom map from: %s\n", mapFile.c_str());
        map.LoadMap(loadMapFromFile(mapFile));
    }
    else
    {
        printw("No map file provided, loading default map.\n");
        map.LoadMap(DEFAULT_MAP);
    }

    printw("Spawning cars...\n");
    auto allIntersections = map.Filter([](auto s) { return s->intersection; });

    for (uint i = 0; i < 20; i++)
    {
        ivec2 randomPos = allIntersections.at(rand() % allIntersections.size())->position;
        map.SpawnCar(randomPos);
    }

    printw("Press any key to start.");
    getch();

    curs_set(0);
    getmaxyx(stdscr, rows, columns);

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

        std::this_thread::sleep_for(Config::screenRefreshStep);
    }

    map.Stop();

    endwin();

    return 0;
}
