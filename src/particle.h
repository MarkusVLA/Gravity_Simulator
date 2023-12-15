#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/vec.h"


#define G 6.67430e-11 // Gravitational constant
#define RADIUS 8

class Particle {
private:
    Vector2<double> position_, velocity_;
    double mass_;
    sf::Color color_;
    double radius_; // Radius of the particle for drawing on screen 

public:
    Particle(): position_(0.0, 0.0), velocity_(0.0, 0.0), mass_(10e9), color_(sf::Color::White), radius_(mass_ / 10e9) { }
    Particle(double mass): position_(0.0, 0.0), velocity_(0.0, 0.0), mass_(mass), color_(sf::Color::White), radius_(mass_ / 10e9) { }
    Particle(double mass, Vector2<double> position): position_(position), velocity_(0.0, 0.0), mass_(mass), color_(sf::Color::White), radius_(mass_ / 10e9) { }
    Particle(double mass, Vector2<double> position, sf::Color color): position_(position), velocity_(0.0, 0.0), mass_(mass), color_(color), radius_(mass_ / 10e9) { }

    ~Particle() { }

    void applyForce(const Vector2<double>& force, double timeStep) {
        Vector2<double> acceleration = force / mass_ * timeStep;
        velocity_ += acceleration; // Update velocity
        updatePosition(timeStep); // Also update the position once the force has been aplied
    }

    void updatePosition(double timeStep) {
        position_ += velocity_ * timeStep; // Update position
    }

    // Getter for velocity
    Vector2<double> getVelocity() const {
        return velocity_;
    }

    void setVelocity(const Vector2<double>& newVelocity) {
        velocity_ = newVelocity;
    }

    Vector2<double> getPosition() const { return position_; }
    double getMass() const { return mass_; }

    Vector2<double> calculateGravitationalForce(double mass, Vector2<double> position){
        Vector2<double> distanceVec = position - position_;
        double distance = distanceVec.Magnitude();
        const double minDistance = 20; // Minimum distance threshold
        distance = std::max(distance, minDistance);
        double forceMagnitude = (G * mass_ * mass) / (distance * distance);
        Vector2<double> force = distanceVec.Normalize() * forceMagnitude;
        return force;
    }
    

    void checkAndWrapBounds(double screenWidth, double screenHeight) {
        if (position_.GetX() < 0) {
            position_.x_ = screenWidth;
        } else if (position_.GetX() > screenWidth) {
            position_.x_ = 0;
        }

        if (position_.GetY() < 0) {
            position_.y_ = screenHeight;
        } else if (position_.GetY() > screenHeight) {
            position_.y_ = 0;
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::CircleShape shape(static_cast<float>(radius_));
        shape.setFillColor(color_);
        shape.setPosition({static_cast<float>(position_.GetX()), static_cast<float>(position_.GetY())});
        window.draw(shape);
    }

    void drawAsPoints(sf::RenderWindow& window, sf::RenderStates states) {
        sf::Vertex point(sf::Vector2f(static_cast<float>(position_.GetX()), static_cast<float>(position_.GetY())), color_);
        window.draw(&point, 1, sf::PrimitiveType::Points, states);
    }

};
