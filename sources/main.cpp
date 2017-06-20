#include "common.hpp"

#include "map.hpp"
#include "car.hpp"

#include <ncurses.h>

#include <fstream>

#include "graph.hpp"
#include "config.hpp"
#include "rendering.hpp"
#include "stats.hpp"
#include <sstream>

using namespace std::literals::chrono_literals;

int columns = 0, rows = 0;

const std::vector<std::string> DEFAULT_MAP = {
    "............",
    ".!>>#>#>>>#.",
    ".v..^ v...v.",
    ".v..^ v...v.",
    ".v..^ v...v.",
    ".v..^ v...v.",
    ".#>>#>#>>>#.",
    ".^..^.v...v.",
    ".#<<#<#<<<#.",
    ".v..^ v...v.",
    ".v..^ #<<<@.",
    ".@>>@ v.....",
    "....^ v.....",
    "....#<#<!...",
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
    srand(time(nullptr));

    initscr();
    getmaxyx(stdscr, rows, columns);
    Renderer renderer(columns, rows);

    printw("Starting...\n");

    Map map;

    if (argc >= 2)
    {
        std::string mapFile(argv[1]);
        printw("Loading custom map from: %s\n", mapFile.c_str());

        try
        {
            map.LoadMap(loadMapFromFile(mapFile));
            printw("Success!\n");
        }
        catch(std::runtime_error& err)
        {
            printw("An error occured while loading the specified map: \n");
            printw(err.what());
            printw("\nProceeding with the default map.\n");
            map.LoadMap(DEFAULT_MAP);
        }
        catch(...)
        {
            printw("An unkown error occured while loading the map.");
            printw("The program will proceed using the default map.");
            map.LoadMap(DEFAULT_MAP);
        }
    }
    else
    {
        printw("No map file provided, loading default map.\n");
        map.LoadMap(DEFAULT_MAP);
    }

    printw("Press any key to start.");
    getch();

    noecho();
    timeout(10);

    clear();

    map.Run();

    while (true)
    {
        for (int x = 0; x < map.Size().x; x++)
        {
            for (int y = 0; y < map.Size().y; y++)
            {
                if (map.Get({x, y})->wall)
                    renderer.DrawableUpdate({x, y}, 3, "#");
            }
        }
        for (auto c : map.GetCars())
        {
            renderer.DrawableUpdate(c->GetPosition(), 2, "O");
        }

        std::string averageTravelTime;
        {
            std::ostringstream ss;
            ss << "Average travel time is: ";
            ss << (float) Statistics::Instance().GetAverageTravelTimeInSeconds();
            ss << " s            ";
            averageTravelTime = ss.str();
        }
        std::string currentCarCount;
        {
            std::ostringstream ss;
            ss << "Current car count: " << map.GetCars().size() << "/" << Config::maxCarsInSimulation << "     ";
            currentCarCount = ss.str();
        }
        std::string totalCarCount;
        {
            std::ostringstream ss;
            ss << "Total car count: " << Statistics::Instance().GetTotalCarsSpawnedCount() << "     ";
            totalCarCount = ss.str();
        }
        std::string simulationRunningTime;
        {
            std::ostringstream ss;
            ss << "Simulation running time is: ";
            ss << (float) Statistics::Instance().GetSimulationRunningTimeInSeconds();
            ss << " s            ";
            simulationRunningTime = ss.str();
        }

        renderer.DrawableUpdate({2, rows - 9}, 1, "Use 'Enter' to quit.");
        renderer.DrawableUpdate({2, rows - 6}, 1, simulationRunningTime.c_str());
        renderer.DrawableUpdate({2, rows - 5}, 1, averageTravelTime.c_str());
        renderer.DrawableUpdate({2, rows - 4}, 1, currentCarCount.c_str());
        renderer.DrawableUpdate({2, rows - 3}, 1, totalCarCount.c_str());

        if (getch() == 10)
        {
            renderer.DrawableUpdate({2, rows - 9}, 1, "Stopping...             ");
            renderer.SwapBuffers();
            refresh();

            break;
        }

        renderer.SwapBuffers();

        refresh();

        std::this_thread::sleep_for(Config::screenRefreshStep);
    }

    map.Stop();

    endwin();

    return 0;
}
