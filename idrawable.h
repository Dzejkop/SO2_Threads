#pragma once

class IDrawable
{
    virtual const char* Image(uvec2 p) const = 0;
    virtual const uvec2& DrawableSize() const = 0;
    virtual const uvec2& DrawablePosition() const = 0;
};