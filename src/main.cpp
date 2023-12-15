
#include "utils/vec.h"
#include <iostream>
#include "particle.h"
#include <SFML/Graphics.hpp>
#include "utils/rand.h"
#include "gridmap.h"
#include <SFML/Graphics.hpp>


#define GRID_SIZE 16
sf::Vector2u windowSize = {1700, 1024};
const double TIME_STEP = 2; // Seconds / Sim frame

int main() {

    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window", sf::Style::Default | sf::Style::Resize);
    // sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window");
    window.setFramerateLimit(200); // frame rate

    std::vector<Particle> particles;
    unsigned int numParticles = 1000; // Number of particles


    // Create particles with random positions and velocities
    for (unsigned int i = 0; i < numParticles; i++) {
        double mass = Random::GetRandomDoubleNormal(10e9, 10e9, i); // Randimized mass for each particle
        Vector2<double> position(Random::GetRandomDoubleUniform(0, windowSize.x, i * 4), Random::GetRandomDoubleUniform(0, windowSize.y, i * 10));
        Vector2<double> vel(Random::GetRandomDoubleNormal(0.4, 0.0, i+2), Random::GetRandomDoubleNormal(0.4, 0.0, i+3));
        Particle particle(mass, position, sf::Color( (int)Random::GetRandomDoubleUniform(0,255, i), (int)Random::GetRandomDoubleUniform(0,255, i + 1),(int)Random::GetRandomDoubleUniform(0,255, i + 2)));
        particle.setVelocity(vel);
        particles.push_back(particle);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                // Update the view to the new size of the window
                sf::FloatRect visibleArea({0.0, 0.0}, {(float) event.size.width, (float) event.size.height});
                window.setView(sf::View(visibleArea));
            }
        }

        



        // Update forces, positions, and check for collisions

        window.clear();
        for (int i = 0; i < particles.size(); i++){
            Particle &part = particles[i];
            Vector2<double> force = {0, 0}; // Start with zero force
            for (int j = 0; j < particles.size(); j++){
                if (i != j){
                    force += part.calculateGravitationalForce(particles[j].getMass(), particles[j].getPosition());
                }
            }
            part.applyForce(force, TIME_STEP);
            part.draw(window); 
        }

        window.display();
    }

    return 0;
}