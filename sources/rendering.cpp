#include "rendering.hpp"

#include <ncurses.h>

Renderer::Renderer(uint screenWidth, uint screenHeight)
    : m_one(screenWidth, std::vector<Pixel>(screenHeight, Pixel()))
    , m_two(m_one)
    , m_width(screenWidth)
    , m_height(screenHeight)
{
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);

    curs_set(0);
}

Renderer::Pixel::Pixel()
    : graphic(" ")
    , color(0)
{
}

void Renderer::SwapBuffers()
{
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            const Pixel& one = m_one.at(x).at(y);
            const Pixel& two = m_two.at(x).at(y);
            if ((one.color != two.color) || (one.graphic != two.graphic))
            {
                DrawPixel(one, {x, y});
            }
        }
    }

    m_two = m_one;
    m_one = std::vector<std::vector<Pixel>>(m_width, std::vector<Pixel>(m_height, Pixel()));
}

void Renderer::DrawableUpdate(const ivec2 &pos, uint color, std::string graphic)
{
    Pixel& pixel = m_one.at(pos.x).at(pos.y);

    pixel.color = color;
    pixel.graphic = graphic;
}

void Renderer::DrawPixel(const Renderer::Pixel &pixel, ivec2 pos)
{
    attron(COLOR_PAIR(pixel.color));
    mvprintw(pos.y, pos.x, pixel.graphic.c_str());
    attroff(COLOR_PAIR(pixel.color));
}
