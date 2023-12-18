#pragma once

#include <SFML/Graphics.hpp>


class Camera {
public:
    Camera(float width, float height) : view(sf::FloatRect({0, 0}, {width, height})) {}

    void handleInput() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            view.move({0, -5 * view.getSize().y / 1000}); // Move up
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            view.move({0, 5 * view.getSize().y / 1000}); // Move down
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            view.move({-5 * view.getSize().x / 1000, 0}); // Move left
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            view.move({5 * view.getSize().x / 1000, 0}); // Move right
        }
    }

    void handleZoom(float delta) {
        if (delta > 0 && view.getSize().x > minZoomLevel) {
            view.zoom(0.95f); // Zoom in
        } else if (delta < 0 && view.getSize().x < maxZoomLevel) {
            view.zoom(1.05f); // Zoom out
        }
    }


    sf::View& getView() {
        return view;
    }

private:
    sf::View view;
    float minZoomLevel = 1000; // Minimum zoom level
    float maxZoomLevel = 100000; // Maximum zoom level
};