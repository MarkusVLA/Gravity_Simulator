#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "vec.h"
#include "../camera.h"

#define RADIUS 10



class ParticleShader {
public:

    sf::Shader shader;
    sf::Shader postProcessingShader;
    sf::RenderStates states;
    sf::RenderStates postProcessingStates;

    ParticleShader() {
        // Load the shaders
        if (!shader.loadFromFile("../src/shaders/particle.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading fragment shader" << std::endl;
        }

        if (!postProcessingShader.loadFromFile("../src/shaders/postprocess.frag", sf::Shader::Fragment)) {
            std::cerr << "Error loading post-processing shader" << std::endl;
            return;
        }

        states.shader = &shader;
        postProcessingStates.shader = &postProcessingShader;
    }

    void setUniforms(sf::Vector2f mousePos, sf::Vector2u windowSize) {
        shader.setUniform("mouse", mousePos);
        shader.setUniform("windowSize", sf::Vector2f(windowSize));
    }

    void setSpeed(float speed) {
        shader.setUniform("u_speed", speed);
    }

    void setColor(sf::Color& color) {
        // Convert from sf::Color to vec4
        sf::Glsl::Vec4 colorVec( color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
        // Set the uniform
        shader.setUniform("u_color", colorVec);
    }

    void drawParticles(sf::RenderWindow& window, Camera& camera, sf::VertexArray& vertices) {
        sf::Vector2f viewSize = camera.getView().getSize();
        sf::RenderTexture renderTexture;

        sf::Vector2u windowSize = window.getSize();

        if (!renderTexture.create({windowSize.x, windowSize.y})) {
            std::cerr << "Failed to create render texture" << std::endl;
            return;
        }

        // Transform the positions of the vertices to the coordinate space of the view
        for (size_t i = 0; i < vertices.getVertexCount(); ++i) {
            sf::Vector2f worldPos = vertices[i].position;
            sf::Vector2i pixelPos = window.mapCoordsToPixel(worldPos, camera.getView());
            sf::Vector2f viewPos(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));
            vertices[i].position = viewPos;
        }

        renderTexture.clear();
        renderTexture.draw(vertices, states); // Draw the particles to the render texture
        renderTexture.display(); // Don't forget to display the render texture

        postProcessingShader.setUniform("u_texture", renderTexture.getTexture());
        postProcessingShader.setUniform("u_texelSize", sf::Vector2f(1.f / viewSize.x, 1.f / viewSize.y));
        postProcessingShader.setUniform("u_radius", RADIUS);

        window.draw(sf::Sprite(renderTexture.getTexture()), postProcessingStates); // Draw the render texture to the window with the post-processing shader
    }
};