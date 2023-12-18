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

    sf::Transform getProjectionMatrix() const {
        sf::Vector2f size = view.getSize();
        sf::Vector2f center = view.getCenter();

        float left = center.x - size.x / 2.0f;
        float right = center.x + size.x / 2.0f;
        float bottom = center.y + size.y / 2.0f;
        float top = center.y - size.y / 2.0f;

        sf::Transform projection;
        projection.translate({left, top});
        projection.scale({2.0f / (right - left), -2.0f / (bottom - top)});
        return projection;
    }

    sf::Transform getViewMatrix() const {
        sf::Transform viewTransform = view.getTransform();
        return viewTransform.getInverse();
    }

private:
    sf::View view;
    float minZoomLevel = 1000; // Minimum zoom level
    float maxZoomLevel = 100000; // Maximum zoom level
};