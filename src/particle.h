#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "utils/vec.h"


#define G 6.67430e-11 // Gravitational constant
#define RADIUS 8



class ParticleShader {
public:

    sf::Shader shader;
    sf::RenderStates states;

    ParticleShader() {
        if (!shader.loadFromFile("../src/shaders/particle.vert", sf::Shader::Vertex)) {
            std::cerr << "Error loading vertex shader" << std::endl;
        }
        if (!shader.loadFromFile("../src/shaders/particle.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading fragment shader" << std::endl;
        }
        states.shader = &shader;
    }

    void setUniforms(sf::Vector2f mousePos, sf::Vector2u windowSize) {
        shader.setUniform("mouse", mousePos);
        shader.setUniform("windowSize", sf::Vector2f(windowSize));
    }

    void setColor(sf::Color& color) {
        // Convert from sf::Color to vec4
        sf::Glsl::Vec4 colorVec(
            color.r / 255.f,
            color.g / 255.f,
            color.b / 255.f,
            color.a / 255.f
        );

        // Set the uniform
        shader.setUniform("u_color", colorVec);
    }
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
        // updatePosition(timeStep); // Also update the position once the force has been aplied
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

    Vector2<double> calculateGravitationalForce(double mass, const Vector2<double>& position) const {
        Vector2<double> distanceVec = position - position_;
        double distanceSquared = distanceVec.SquaredMagnitude();
        const double minDistanceSquared = 1000;  // Minimum distance threshold squared
        distanceSquared = std::max(distanceSquared, minDistanceSquared);
        double forceMagnitude = (G * mass_ * mass) / distanceSquared;
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

    friend void drawParticlesAsSpheres(sf::RenderWindow& window, ParticleShader &particleShader, std::vector<Particle>& particles) {
        // Iterate over each particle
        for (Particle &particle : particles) {
            // Create a circle to represent the particle
            sf::CircleShape circle(particle.radius_);
            // Set the position of the circle
            circle.setPosition({static_cast<float>(particle.position_.GetX()), static_cast<float>(particle.position_.GetY())});

            // Convert the particle's color to a vec4 and pass it to the shader
            particleShader.setColor(particle.color_);
        
            // Draw the circle with the shader
            window.draw(circle, &particleShader.shader);
        }
    }


    friend void processParticles(std::vector<Particle>& particles, int start, int end, double timeStep) {
    for (int i = start; i < end; ++i) {
        // Calculate the force on the particle
        Particle &part = particles[i];
        Vector2<double> force = {0, 0}; // Start with zero force
        for (int j = i + 1; j < particles.size(); j++){
            Vector2<double> force_ij = part.calculateGravitationalForce(particles[j].getMass(), particles[j].getPosition());
            force += force_ij;
            particles[j].applyForce(force_ij * -1, timeStep); // Apply the opposite force to the other particle
        }

        part.applyForce(force, timeStep); // Apply the force to the particle
    }

    for (Particle &part : particles) {
        part.updatePosition(timeStep); // Update the position of the particle
    }
}

    friend std::ostream& operator<<(std::ostream& os, const Particle& particle) {
        os << "Particle: " << particle.position_ << " " << particle.velocity_ << " " << particle.mass_;
        return os;
    }
};
