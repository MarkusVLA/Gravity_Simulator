// quadtree data structure that is used to store particles in a 2D space and calculate gravitational force.
#pragma once

#include <algorithm>
#include <vector>
#include "../particle.h"
#include <SFML/Graphics.hpp>
#include "vec.h"
#include <memory>

class Rectangle {
public:
    double x, y, width, height;

    Rectangle(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}

    bool contains(const Vector2<double>& point) const {
        return point.GetX() >= x && point.GetX() <= x + width && point.GetY() >= y && point.GetY() <= y + height;
    }

    void draw(sf::RenderWindow& window) {
        sf::RectangleShape rect;
        rect.setPosition(sf::Vector2f({static_cast<float>(x), static_cast<float>(y)}));
        rect.setSize(sf::Vector2f({static_cast<float>(width), static_cast<float>(height)}));
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color(0, 255, 0, 64));
        rect.setOutlineThickness(0.5);
        window.draw(rect);
    }
};


class QuadTree {
private:
    Rectangle bounds_; // Bounds of the tree
    int capacity_; // Maximum number of particles in a node 
    std::vector<Particle> particles_; // Particles the node
    double mass_; // Total mass of the node
    Vector2<double> centerOfMass_; // Center of mass of the node
    bool divided_ = false; // Is the node divided?

    // Branches of the tree
    std::unique_ptr<QuadTree> northWest_;
    std::unique_ptr<QuadTree> northEast_;
    std::unique_ptr<QuadTree> southWest_;
    std::unique_ptr<QuadTree> southEast_;

public:

    // Create new quadtree
    QuadTree(const Rectangle& bounds, int capacity);
    // Delete quadtree and all its children
    ~QuadTree();

    // Getters
    double getMass() const;
    Rectangle getBounds() const;
    Vector2<double> getCenterOfMass() const;
    bool isDivided() const;

    // Update the bounds of the tree
    void setBounds(const Rectangle& bounds);
    // Subdivide the tree into four branches
    void subdivide();
    // Insert a particle into the tree 
    bool insert(Particle& particle);
    // draw the tree on SFML window
    void draw(sf::RenderWindow& window);
    // Clear the tree
    void clear(); 
    //calculate the total mass and center of mass of the node
    void calculateMassAndCenterOfMass();
    // Update the tree by recalculating the mass and center of mass of each node
    void updateTreeMass();
    // Calculate the gravitational force of QuadTree on a particle
    Vector2<double> calculateGravitationalForce(Particle& particle);
    // Calculate the gravitational force of a single mass
    Vector2<double> calculateForceForSingleMass(Particle& particle, Vector2<double> distanceVec, double distanceSquared);
};
