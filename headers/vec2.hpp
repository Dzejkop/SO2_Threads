#pragma once

template <typename T>
struct vec2
{
    vec2(T x, T y)
        : x(x)
        , y(y)
    {}

    vec2()
        : x(0)
        , y(0)
    {
    }

    vec2(const vec2& other) = default;

    vec2 operator+(const vec2& other) const
    {
        return vec2(this->x + other.x, this->y + other.y);
    }

    vec2 operator-() const
    {
        return vec2(-x, -y);
    }

    vec2 operator+=(const vec2& other)
    {
        *this = *this + other;
        return *this;
    }

    vec2 operator-(const vec2& other) const
    {
        return *this + -other;
    }

    bool operator==(const vec2& other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    bool operator!=(const vec2& other) const
    {
        return !(*(this) == other);
    }

    T x;
    T y;
};
