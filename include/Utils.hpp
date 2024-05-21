#pragma once

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Vector2.hpp"
#include "Vector2f.hpp"

namespace utils
{
    inline float hires_time_in_seconds()
    {
        float t = SDL_GetTicks64();
        t *= 0.001f;

        return t;
    }

    // int getRefreshRate(SDL_Window* p_window)
    // {
    //     int displayIndex = SDL_GetWindowDisplayIndex(p_window);
    //     SDL_DisplayMode mode;
    //     SDL_GetDisplayMode(displayIndex, 0, &mode);
    //     return mode.refresh_rate;
    // }

    inline float getRotationAngle(Vector2 p_mouseState, Vector2 p_rotationPoint) {
        return std::atan2(p_mouseState.y - p_rotationPoint.y, p_mouseState.x - p_rotationPoint.x) * (180 / M_PI);
    }

    inline Vector2 applyRotationMatrix(Vector2 p_vec, float p_angle) {
        // convert to radians
        float radians = p_angle * (M_PI / 180.0f);
        float cosTheta = std::cos(radians);
        float sinTheta = std::sin(radians);
        
        // rotation matrix
        float newX = p_vec.x * cosTheta - p_vec.y * sinTheta;
        float newY = p_vec.x * sinTheta + p_vec.y * cosTheta;
        
        // convert back to integers, using rounding to nearest integer
        return Vector2(static_cast<int>(std::round(newX)), static_cast<int>(std::round(newY)));
    }

    inline Vector2f applyRotationMatrixf(Vector2f p_vec, float p_angle) {
        // convert to radians
        float radians = p_angle * (M_PI / 180.0f);
        float cosTheta = std::cos(radians);
        float sinTheta = std::sin(radians);
        
        // rotation matrix
        float newX = p_vec.x * cosTheta - p_vec.y * sinTheta;
        float newY = p_vec.x * sinTheta + p_vec.y * cosTheta;

        return Vector2f(newX, newY);
    }
}