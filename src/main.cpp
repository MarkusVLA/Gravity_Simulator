
#include "utils/vec.h"
#include <iostream>
#include "particle.h"
#include <SFML/Graphics.hpp>
#include "utils/rand.h"
#include "utils/gui.h"
#include "utils/threadpool.h"
#include <thread>
#include "camera.h"
#include "utils/quadtree.h"


const int NUM_THREADS = std::thread::hardware_concurrency() - 2; // Number of threads to use for simulation
sf::Vector2u windowSize = {1800, 1100};
volatile double TIME_STEP = 0.0f; // Seconds / Sim frame

int main() {
    sf::RenderWindow window(sf::VideoMode(windowSize, 1), "SFML Window", sf::Style::Default | sf::Style::Resize);
    window.setFramerateLimit(60); 

    Camera camera(windowSize.x, windowSize.y); // Create a Camera object

    std::vector<Particle> particles;
    unsigned int numParticles = 20e3; // Number of particles

    QuadTree testQuadTree(Rectangle(0, 0, windowSize.x, windowSize.y), 16); // Create a QuadTree object

    Slider slider(100, 100, 200, 20); // Create a Slider object
    bool dragging = false; // Is the user dragging the slider?

    sf::Font font;
        if (!font.loadFromFile("../src/fonts/tuffy.ttf"))
            return EXIT_FAILURE;

    sf::Text text(font, "Time Step", 20);


    // Define centers for the two balls
    Vector2<double> center1(windowSize.x / 4, windowSize.y / 2);
    Vector2<double> center2(3 * windowSize.x / 4, windowSize.y / 2);
    double radius = 200; // Radius of the balls

    for (unsigned int i = 0; i < numParticles; i++) {
        double mass = Random::GetRandomDoubleNormal(10e9, 10e9, i); // Randomized mass for each particle

        // Choose a center based on whether i is odd or even
        Vector2<double> center = (i % 2 == 0) ? center1 : center2;

        // Generate a random angle and distance within the ball radius
        double angle = Random::GetRandomDoubleUniform(0, 2 * M_PI, i);
        double distance = Random::GetRandomDoubleUniform(0, radius, i);

        // Calculate position based on polar coordinates
        Vector2<double> position(center.GetX() + distance * cos(angle), center.GetY() + distance * sin(angle));

        // Create random velocities (you might want to adjust this part based on your simulation needs)
        Vector2<double> vel(Random::GetRandomDoubleNormal(0.4, 0.0, i+2), Random::GetRandomDoubleNormal(0.4, 0.0, i+3));

        // Create the particle
        Particle particle(mass, position, sf::Color((int)Random::GetRandomDoubleUniform(0,255, i), (int)Random::GetRandomDoubleUniform(0,255, i + 1), (int)Random::GetRandomDoubleUniform(0,255, i + 2)));
        particle.setVelocity(vel);
        particles.push_back(particle);
    }

    // Create a thread pool with NUM_THREADS threads
    ThreadPool pool(NUM_THREADS);

    // Shader object for drawing particles
    ParticleShader shader;

    while (window.isOpen()) {
        sf::Event event; 
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                camera.handleZoom(event.mouseWheelScroll.delta); // Handle zooming
            } else if (event.type == sf::Event::Resized) {
                // Update the view to the new size of the window
                sf::FloatRect visibleArea({0.0, 0.0}, {(float) event.size.width, (float) event.size.height});
                window.setView(sf::View(visibleArea));
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (slider.contains(event.mouseButton.x, event.mouseButton.y)) { dragging = true; }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                dragging = false;
            } else if (event.type == sf::Event::MouseMoved && dragging) {
                slider.moveThumb(event.mouseMove.x);
                TIME_STEP = slider.getValue() * 2; // Update TIME_STEP based on the slider value
            }
        }

       window.clear();
        camera.handleInput(); // Handle camera movement
        window.setView(camera.getView()); // Set the view to the camera view

        // Barnes-Hut Algorithm
        testQuadTree.clear();

        for (auto& particle : particles) {
            testQuadTree.insert(particle);
        }

        // Update the mass and center of mass of the QuadTree
        testQuadTree.updateTreeMass();

        // Parallel calculation of gravitational forces
        std::vector<Vector2<double>> forces(particles.size());
        for (size_t i = 0; i < particles.size(); ++i) {
            pool.enqueue([&testQuadTree, &particles, &forces, i] {
                forces[i] = testQuadTree.calculateGravitationalForce(particles[i]);
            });
        }
        pool.wait_until_empty(); // Wait for all force calculations to complete

        // Apply forces and update positions
        for (size_t i = 0; i < particles.size(); ++i) {
            particles[i].applyForce(forces[i], TIME_STEP);
            particles[i].updatePosition(TIME_STEP);
        }

        // Draw the particles
        drawParticles(window, shader, particles, camera);

        // draw UI
        window.setView(window.getDefaultView());
        text.setString("Time Step: " + std::to_string(TIME_STEP) + "s" + "\n" + "Number of Particles: " + std::to_string(numParticles) + "\n" + "Threads " + std::to_string(NUM_THREADS));
        window.draw(text);
        slider.draw(window); // Draw the slider
        window.display();
    }

    return 0;
}