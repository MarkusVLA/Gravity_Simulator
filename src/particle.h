#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/vec.h"


#define G 6.67430e-11 // Gravitational constant
#define RADIUS 8



class ParticleShader {
public:

    ParticleShader() {
        if (!shader.loadFromFile("../src/shaders/particle.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading shader" << std::endl;
        }
        states.shader = &shader;
    }

    void setUniforms(sf::Vector2f mousePos, sf::Vector2u windowSize) {
        shader.setUniform("mouse", mousePos);
        shader.setUniform("windowSize", sf::Vector2f(windowSize));
    }
    sf::Shader shader;
    sf::RenderStates states;
};


class Particle {
private:
    Vector2<double> position_, velocity_;
    double mass_;
    sf::Color color_;
    double radius_; // Radius of the particle for drawing on screen 

public:

    Particle(): position_(0.0, 0.0), velocity_(0.0, 0.0), mass_(10e9), color_(sf::Color::White), radius_(mass_ / 10e9) {}
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
    

    friend std::ostream& operator<<(std::ostream& os, const Particle& particle) {
        os << "Particle: " << particle.position_ << " " << particle.velocity_ << " " << particle.mass_;
        return os;
    }


    friend void drawParticlesAsPointCloud(sf::RenderWindow& window, ParticleShader &particleShader, std::vector<Particle>& particles) {
        // Create a buffer to store the positions and colors of the particles
        sf::VertexArray vertices(sf::PrimitiveType::Points, particles.size());

        // Update the buffer with the new positions and colors of the particles
        for (size_t i = 0; i < particles.size(); ++i) {
            vertices[i].position = sf::Vector2f(particles[i].position_.GetX(), particles[i].position_.GetY());
            vertices[i].color = particles[i].color_;
        }

        // Draw the particles
        window.draw(vertices, particleShader.states);
    }


    // friend void drawParticlesAsPointCloud(sf::RenderWindow& window, ParticleShader &shader, std::vector<Particle>& particles) {
    //     // Create a buffer to store the positions and colors of the particles
    //     //std::vector<sf::Vertex> vertices(particles.size());
    //     sf::VertexArray vertexArray(sf::PrimitiveType::Points, particles.size());

    //     // Update the buffer with the new positions and colors of the particles
    //     for (size_t i = 0; i < particles.size(); ++i) {
    //         vertices[i].position = sf::Vector2f(particles[i].position_.GetX(), particles[i].position_.GetY());
    //         vertices[i].color = particles[i].color_;
    //     }

    //     // Draw the particles
    //     window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Points, shader.states);
    // }
};
