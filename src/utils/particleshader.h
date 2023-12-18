#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "vec.h"
#include "../camera.h"


class ParticleShader {
public:
    sf::Shader shader;
    sf::RenderStates states;

    ParticleShader() {
        // Load the particle shader
        if (!shader.loadFromFile("../src/shaders/particle.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading fragment shader" << std::endl;
        }
        states.shader = &shader;
    }

    void setUniforms(sf::Vector2f mousePos, sf::Vector2u windowSize) {
        shader.setUniform("mouse", mousePos);
        shader.setUniform("windowSize", sf::Vector2f(windowSize));
    }

    void setSpeed(float speed) {
        shader.setUniform("u_speed", speed);
    }

    void setColor(sf::Color& color) {
        sf::Glsl::Vec4 colorVec(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
        shader.setUniform("u_color", colorVec);
    }

    void drawParticle(sf::RenderWindow& window, sf::VertexArray& vertices) {
        window.draw(vertices, states); // Draw the particles directly to the window
    }

    void drawParticles(sf::RenderWindow& window, Camera& camera, sf::VertexArray vertices) {
        // Transform the positions of the vertices to the coordinate space of the view
        window.draw(vertices, states); // Draw the particles directly to the window
    }
};
