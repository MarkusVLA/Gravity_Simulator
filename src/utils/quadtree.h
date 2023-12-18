#pragma once
#include <algorithm>
#include <vector>
#include "../particle.h"
#include <SFML/Graphics.hpp>
#include "vec.h"

class Rectangle {
public:
    double x, y, width, height;

    Rectangle(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}

    bool contains(const Vector2<double>& point) const {
        return point.GetX() >= x && point.GetX() <= x + width && point.GetY() >= y && point.GetY() <= y + height;
    }
};

