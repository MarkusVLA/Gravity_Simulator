#include "quadtree.h"

QuadTree::QuadTree(const Rectangle& bounds, int capacity) : bounds_(bounds), capacity_(capacity) {}

QuadTree::~QuadTree() { clear(); }

double QuadTree::getMass() const { return mass_; }
Rectangle QuadTree::getBounds() const { return bounds_; }
Vector2<double> QuadTree::getCenterOfMass() const { return centerOfMass_; }
bool QuadTree::isDivided() const { return divided_; }


void QuadTree::subdivide() {
    double x = bounds_.x;
    double y = bounds_.y;
    double width = bounds_.width;
    double height = bounds_.height;

    northWest_ = std::make_unique<QuadTree>(Rectangle(x, y, width / 2, height / 2), capacity_);
    northEast_ = std::make_unique<QuadTree>(Rectangle(x + width / 2, y, width / 2, height / 2), capacity_);
    southWest_ = std::make_unique<QuadTree>(Rectangle(x, y + height / 2, width / 2, height / 2), capacity_);
    southEast_ = std::make_unique<QuadTree>(Rectangle(x + width / 2, y + height / 2, width / 2, height / 2), capacity_);
    divided_ = true;
}


bool QuadTree::insert(Particle &particle) {
    if (!bounds_.contains(particle.getPosition())) {
        return false;
    }

    if (!divided_) {
        if (static_cast<int>(particles_.size()) < capacity_) {
            // If the current node has space, just add the particle
            particles_.push_back(particle);
            return true;
        } else {
            // If the current node is at capacity, subdivide and then try inserting again
            subdivide();
            // Note: There is no need to reinsert the existing particles 
            // as they are already correctly positioned within the current bounds.
        }
    }

    // After subdivision, try inserting the particle into one of the new quadrants
    if (northWest_->insert(particle)) return true;
    if (northEast_->insert(particle)) return true;
    if (southWest_->insert(particle)) return true;
    if (southEast_->insert(particle)) return true;

    // This should not happen, but return false if insertion failed for some reason
    return false;
}


void QuadTree::calculateMassAndCenterOfMass() {
    mass_ = 0;
    centerOfMass_ = Vector2<double>();
    for (Particle& particle : particles_) {
        mass_ += particle.getMass();
        centerOfMass_ += particle.getPosition() * particle.getMass();
    }
    centerOfMass_ /= mass_;
}

void QuadTree::updateTreeMass() {
    calculateMassAndCenterOfMass();
    if (divided_) {
        northWest_->updateTreeMass();
        northEast_->updateTreeMass();
        southWest_->updateTreeMass();
        southEast_->updateTreeMass();
    }
}

Vector2<double> QuadTree::calculateGravitationalForce(Particle& particle) {
    if (mass_ == 0) { // Avoid self-interaction
        return Vector2<double>();
    }

    Vector2<double> distanceVec = particle.getPosition() - centerOfMass_;
    double distanceSquared = distanceVec.SquaredMagnitude();
    double widthSquared = bounds_.width * bounds_.width;

    // Check if we can treat this node as a single mass (using squared values to avoid sqrt)
    if (widthSquared / distanceSquared < 0.8 * 0.8) {
        return calculateForceForSingleMass(particle, distanceVec, distanceSquared);
    }

    // Recursive force calculation for divided nodes
    Vector2<double> force;
    if (divided_) {
        force += northWest_->calculateGravitationalForce(particle);
        force += northEast_->calculateGravitationalForce(particle);
        force += southWest_->calculateGravitationalForce(particle);
        force += southEast_->calculateGravitationalForce(particle);
    }
    return force;
}

    Vector2<double> QuadTree::calculateForceForSingleMass(Particle& particle, Vector2<double> distanceVec, double distanceSquared) {
        const double minDistanceSquared = 1000; // Minimum distance threshold squared
        distanceSquared = std::max(distanceSquared, minDistanceSquared);
        double forceMagnitude = (G * mass_ * particle.getMass()) / distanceSquared;
        return distanceVec.Normalize() * -1 * forceMagnitude;
    }



void QuadTree::draw(sf::RenderWindow& window) {
    bounds_.draw(window);
    if (divided_) {
        northWest_->draw(window);
        northEast_->draw(window);
        southWest_->draw(window);
        southEast_->draw(window);
    }
}

void QuadTree::clear() {
    // Clear the data for this node
    particles_.clear();
    mass_ = 0;
    centerOfMass_ = Vector2<double>();
    divided_ = false;

    // Recursively clear child nodes and reset them
    if (northWest_) {
        northWest_->clear();
        northWest_.reset();
    }
    if (northEast_) {
        northEast_->clear();
        northEast_.reset();
    }
    if (southWest_) {
        southWest_->clear();
        southWest_.reset();
    }
    if (southEast_) {
        southEast_->clear();
        southEast_.reset();
    }
}
