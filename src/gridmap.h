#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "particle.h"


class Grid {
private:
    double gridSize_;
    Vector2<double> gridCoords_;
    std::vector<Particle> particles_;
    double mass_;

public:
    
    Grid() : gridCoords_({0,0}), mass_(0), gridSize_(16) { }
    Grid(Vector2<double> gridCoords) : gridCoords_(gridCoords), mass_(0), gridSize_(16) { }

    void addParticle(const Particle& particle) {
        particles_.push_back(particle);
        mass_ += particle.getMass();
    }

    void ClearParticles(void){
        particles_.clear();
        mass_ = 0;
    }

    double getMass() const { return mass_; }
    const std::vector<Particle>& getParticles() const { return particles_; }
    
    Vector2<double> getCenter() const {
        double centerX = gridCoords_.GetX() * gridSize_ + gridSize_ / 2.0;
        double centerY = gridCoords_.GetY() * gridSize_ + gridSize_ / 2.0;
        return Vector2<double>(centerX, centerY);
    }
};
