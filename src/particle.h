#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/vec.h"
// #include "utils/quadtree.h"
#include "camera.h"
#include "utils/particleshader.h"

#define G 6.67430e-11 // Gravitational constant

class Particle {
private: 
    Vector2<double> position_, velocity_;
    double mass_;
    sf::Color color_;

public:

    Particle(): position_(0.0, 0.0), velocity_(0.0, 0.0), mass_(10e9), color_(sf::Color::White) { }
    Particle(double mass): position_(0.0, 0.0), velocity_(0.0, 0.0), mass_(mass), color_(sf::Color::White) { }
    Particle(double mass, Vector2<double> position): position_(position), velocity_(0.0, 0.0), mass_(mass), color_(sf::Color::White) { }
    Particle(double mass, Vector2<double> position, sf::Color color): position_(position), velocity_(0.0, 0.0), mass_(mass), color_(color) { }

    ~Particle() { }
    void updatePosition(double timeStep) { position_ += velocity_ * timeStep; } // Update position}
    Vector2<double> getVelocity() const { return velocity_; }
    void setVelocity(const Vector2<double>& newVelocity) { velocity_ = newVelocity; }
    Vector2<double> getPosition() const { return position_; }
    double getMass() const { return mass_; }

    void applyForce(const Vector2<double>& force, double timeStep) {
        Vector2<double> acceleration = force / mass_ * timeStep;
        velocity_ += acceleration; // Update velocity
        // updatePosition(timeStep); // Also update the position once the force has been aplied
    }

    Vector2<double> calculateGravitationalForce(double mass, const Vector2<double>& position) const {
        Vector2<double> distanceVec = position - position_;
        double distanceSquared = distanceVec.SquaredMagnitude();
        const double minDistanceSquared = 1000;  // Minimum distance threshold squared
        distanceSquared = std::max(distanceSquared, minDistanceSquared);
        double forceMagnitude = (G * mass_ * mass) / distanceSquared;
        Vector2<double> force = distanceVec.Normalize() * forceMagnitude;
        return force;
    }

    friend void drawParticles(sf::RenderWindow& window, ParticleShader &particleShader, std::vector<Particle>& particles, Camera& camera) {
        // Create a vertex array for all particles
        sf::VertexArray vertices(sf::PrimitiveType::Points, particles.size());
        for (size_t i = 0; i < particles.size(); ++i) {
            const Particle &particle = particles[i];
            float speed = particle.velocity_.SquaredMagnitude();
            // Set the position of the vertex
            vertices[i].position = sf::Vector2f(particle.getPosition().GetX(), particle.getPosition().GetY());
            vertices[i].texCoords = sf::Vector2f(speed, 0); // Misuse texCoords to store speed
        }

        particleShader.drawParticle(window, vertices);
    }

    friend std::ostream& operator<<(std::ostream& os, const Particle& particle) {
        os << "Particle: " << particle.position_ << " " << particle.velocity_ << " " << particle.mass_;
        return os;
    }
};
