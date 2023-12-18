
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

#define GRID_SIZE 16
#define NUM_THREADS std::thread::hardware_concurrency() - 2 // Number of threads to use for simulation
sf::Vector2u windowSize = {1800, 1000};
volatile double TIME_STEP = 0.0f; // Seconds / Sim frame

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(windowSize, 32), "SFML Window", sf::Style::Default | sf::Style::Resize, settings);
    window.setFramerateLimit(60); 

    Camera camera(windowSize.x, windowSize.y); // Create a Camera object

    std::vector<Particle> particles;
    unsigned int numParticles = 1000; // Number of particles

    Slider slider(100, 100, 200, 20); // Create a Slider object
    bool dragging = false; // Is the user dragging the slider?

    sf::Font font;
        if (!font.loadFromFile("../src/fonts/tuffy.ttf"))
            return EXIT_FAILURE;

    sf::Text text(font, "Time Step", 20);


    // Create particles with random positions and velocities
    for (unsigned int i = 0; i < numParticles; i++) {
        double mass = Random::GetRandomDoubleNormal(10e9, 10e9, i); // Randimized mass for each particle
        Vector2<double> position(Random::GetRandomDoubleUniform(windowSize.x * 1/3 , windowSize.x * 2/3, i * 4), Random::GetRandomDoubleUniform(windowSize.y * 1/3 , windowSize.y * 2/3, i * 11));
        Vector2<double> vel(Random::GetRandomDoubleNormal(0.4, 0.0, i+2), Random::GetRandomDoubleNormal(0.4, 0.0, i+3));
        Particle particle(mass, position, sf::Color((int)Random::GetRandomDoubleUniform(0,255, i), (int)Random::GetRandomDoubleUniform(0,255, i + 1),(int)Random::GetRandomDoubleUniform(0,255, i + 2)));
        particle.setVelocity(Vector2<double>());
        particles.push_back(particle);
    }

    // Create a thread pool with NUM_THREADS threads
    ThreadPool pool(NUM_THREADS);
    int particlesPerThread = numParticles / NUM_THREADS;

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

        camera.handleInput(); // Handle camera movement

        for (int i = 0; i < NUM_THREADS; i++) {
            int start = i * particlesPerThread;
            int end = (i + 1) * particlesPerThread;
            if (i == NUM_THREADS - 1) {
                end = numParticles;
            }
            pool.enqueue([&, start, end] {
                processParticles(particles, start, end, TIME_STEP);
            });
        }

       
        window.clear();

        window.setView(camera.getView()); // Set the view to the camera view

        // Draw the particles
        drawParticles(window, shader, particles, camera);


        // draw UI
        window.setView(window.getDefaultView());
        text.setString("Time Step: " + std::to_string(TIME_STEP) + "s" + "\n" + "Number of Particles: " + std::to_string(numParticles) + "\n" + "Threads " + std::to_string(NUM_THREADS) );
        window.draw(text);
        slider.draw(window); // Draw the slider
        window.display();
    }

    return 0;
}