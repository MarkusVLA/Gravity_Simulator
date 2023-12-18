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

        // if (!shader.loadFromFile("../src/shaders/particle.vert", "../src/shaders/particle.frag")) {
        //     std::cerr << "Error loading shaders" << std::endl;
        //     // Handle error, possibly exit
        // }

        if (!shader.loadFromFile("../src/shaders/particle.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading shaders" << std::endl;
            // Handle error, possibly exit
        }

        states.shader = &shader;
    }

    void setViewMatrix(const sf::Transform& viewMatrix) {
        shader.setUniform("u_viewMatrix", sf::Glsl::Mat4(viewMatrix.getMatrix()));
    }

    void setProjectionMatrix(const sf::Transform& projectionMatrix) {
        shader.setUniform("u_projectionMatrix", sf::Glsl::Mat4(projectionMatrix.getMatrix()));
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
