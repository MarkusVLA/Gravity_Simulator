#include "quadtree.h"

QuadTree::QuadTree(const Rectangle& bounds, int capacity) : bounds_(bounds), capacity_(capacity) {}

QuadTree::~QuadTree() {
    delete northWest_;
    delete northEast_;
    delete southWest_;
    delete southEast_;
}

double QuadTree::getMass() const { return mass_; }
Rectangle QuadTree::getBounds() const { return bounds_; }
Vector2<double> QuadTree::getCenterOfMass() const { return centerOfMass_; }
bool QuadTree::isDivided() const { return divided_; }


void QuadTree::QuadTree::subdivide() {
    double x = bounds_.x;
    double y = bounds_.y;
    double width = bounds_.width;
    double height = bounds_.height;
    // Check tha these are correct!
    northWest_ = new QuadTree(Rectangle(x, y, width / 2, height / 2), capacity_);
    northEast_ = new QuadTree(Rectangle(x + width / 2, y, width / 2, height / 2), capacity_);
    southWest_ = new QuadTree(Rectangle(x, y + height / 2, width / 2, height / 2), capacity_);
    southEast_ = new QuadTree(Rectangle(x + width / 2, y + height / 2, width / 2, height / 2), capacity_);
    divided_ = true;

}

bool QuadTree::insert(Particle &particle) {
    
    if (!bounds_.contains(particle.getPosition())) {
        return false;
    }

    if (static_cast<int>(particles_.size()) < capacity_) {
        // Insert the particle into the same node
        particles_.push_back(particle);
        return true;
    }

    if (!divided_) {
        // subdivide the tree
        subdivide();
    }

    if (northWest_->insert(particle)) return true;
    if (northEast_->insert(particle)) return true;
    if (southWest_->insert(particle)) return true;
    if (southEast_->insert(particle)) return true;
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

Vector2<double> QuadTree::calculateGravitationalForce(Particle& Particle) {
    if (mass_ == 0) {
        return Vector2<double>();
    } 

    Vector2<double> distanceVec = Particle.getPosition() - centerOfMass_;
    double distanceSquared = distanceVec.SquaredMagnitude();

    // Check if we can treat this node as a single mass
    if ((bounds_.width / sqrt(distanceSquared)) < 0.75) {
        return calculateForceForSingleMass(Particle, distanceVec); // OPTIMISE THIS
    }

    // Original force calculation logic for divided nodes
    Vector2<double> force;
    if (divided_) {
        force += northWest_->calculateGravitationalForce(Particle);
        force += northEast_->calculateGravitationalForce(Particle);
        force += southWest_->calculateGravitationalForce(Particle);
        force += southEast_->calculateGravitationalForce(Particle);
    }
    return force;
}

Vector2<double> QuadTree::calculateForceForSingleMass(Particle& Particle, Vector2<double> distanceVec) {
    double distanceSquared = distanceVec.SquaredMagnitude();
    const double minDistanceSquared = 1000;  // Minimum distance threshold squared
    distanceSquared = std::max(distanceSquared, minDistanceSquared);
    double forceMagnitude = (G * mass_ * Particle.getMass()) / distanceSquared;
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

    // Recursively clear child nodes and delete them
    if (northWest_) { northWest_->clear(); delete northWest_; northWest_ = nullptr; }
    if (northEast_) { northEast_->clear(); delete northEast_; northEast_ = nullptr; }
    if (southWest_) { southWest_->clear(); delete southWest_; southWest_ = nullptr; }
    if (southEast_) { southEast_->clear(); delete southEast_; southEast_ = nullptr; }
}
