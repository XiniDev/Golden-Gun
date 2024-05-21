#pragma once

#include <cmath>
#include <SDL2/SDL.h>

#include <iostream>
#include <string>

struct Vector2
{
    Vector2()
        :x(0.0f), y(0.0f)
    {}

    Vector2(int p_x, int p_y)
        :x(p_x), y(p_y)
    {}

    // overload operators on constants

    Vector2 operator+(int p_c)
    {
        return Vector2(x + p_c, y + p_c);
    }

    friend Vector2 operator+(int p_c, Vector2& p_vec)
    {
        return Vector2(p_c + p_vec.x, p_c + p_vec.y);
    }

    Vector2 operator+(int p_c) const
    {
        return Vector2(x + p_c, y + p_c);
    }

    friend Vector2 operator+(int p_c, const Vector2& p_vec)
    {
        return Vector2(p_c + p_vec.x, p_c + p_vec.y);
    }

    Vector2 operator-(int p_c)
    {
        return Vector2(x - p_c, y - p_c);
    }

    friend Vector2 operator-(int p_c, Vector2& p_vec)
    {
        return Vector2(p_c - p_vec.x, p_c - p_vec.y);
    }

    Vector2 operator-(int p_c) const
    {
        return Vector2(x - p_c, y - p_c);
    }

    friend Vector2 operator-(int p_c, const Vector2& p_vec)
    {
        return Vector2(p_c - p_vec.x, p_c - p_vec.y);
    }

    Vector2 operator*(int p_c)
    {
        return Vector2(x * p_c, y * p_c);
    }

    friend Vector2 operator*(int p_c, Vector2& p_vec)
    {
        return Vector2(p_c * p_vec.x, p_c * p_vec.y);
    }

    Vector2 operator*(int p_c) const
    {
        return Vector2(x * p_c, y * p_c);
    }

    friend Vector2 operator*(int p_c, const Vector2& p_vec)
    {
        return Vector2(p_c * p_vec.x, p_c * p_vec.y);
    }

    Vector2 operator/(int p_c)
    {
        return Vector2(x / p_c, y / p_c);
    }

    friend Vector2 operator/(int p_c, Vector2& p_vec)
    {
        return Vector2(p_c / p_vec.x, p_c / p_vec.y);
    }

    Vector2 operator/(int p_c) const
    {
        return Vector2(x / p_c, y / p_c);
    }

    friend Vector2 operator/(int p_c, const Vector2& p_vec)
    {
        return Vector2(p_c / p_vec.x, p_c / p_vec.y);
    }

    // overload operators for Vector2

    Vector2 operator+(const Vector2& p_vec)
    {
        return Vector2(x + p_vec.x, y + p_vec.y);
    }

    Vector2 operator-(const Vector2& p_vec)
    {
        return Vector2(x - p_vec.x, y - p_vec.y);
    }

    Vector2 operator*(const Vector2& p_vec)
    {
        return Vector2(x * p_vec.x, y * p_vec.y);
    }

    Vector2& operator+=(const Vector2& p_vec)
    {
        x += p_vec.x;
        y += p_vec.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& p_vec)
    {
        x -= p_vec.x;
        y -= p_vec.y;
        return *this;
    }

    Vector2& operator*=(const Vector2& p_vec)
    {
        x *= p_vec.x;
        y *= p_vec.y;
        return *this;
    }

    friend bool operator==(const Vector2& p_vec1, const Vector2& p_vec2)
    {
        return p_vec1.x == p_vec2.x && p_vec1.y == p_vec2.y;
    }

    // to string and print

    std::string toString()
    {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }

    void print()
    {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }

    int x, y;
};