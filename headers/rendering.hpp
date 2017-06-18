#pragma once
#include "common.hpp"

class Renderer
{
public:
    Renderer(uint screenWidth, uint screenHeight);

    struct Pixel
    {
        Pixel();
        uint color;
        std::string graphic;
    };

    typedef std::vector<std::vector<Pixel>> Buffer;

    void SwapBuffers();
    void DrawableUpdate(const ivec2& pos, uint color, std::string graphic);

private:

    void DrawPixel(const Pixel& pixel, ivec2 pos);

    Buffer m_one;
    Buffer m_two;

    uint m_width;
    uint m_height;
};
