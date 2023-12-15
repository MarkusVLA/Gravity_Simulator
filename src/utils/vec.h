/**
 * Vector classes for vector math
*/

#pragma once

#include <iostream>
#include <cmath>

template <typename Type>
struct Vec2 {
public:
    Type x_, y_;
    Vec2(): x_(0.0), y_(0.0) { }
    Vec2(Type x, Type y): x_(x), y_(y) { }
};


template <typename Type>
class Vector2: public Vec2<Type> {

public:

    // Constructors
    Vector2(Type x, Type y): Vec2<Type>(x, y) { }
    Vector2(const Vector2<Type> &constructorVector): Vec2<Type>(constructorVector.GetX(), constructorVector.GetY()) { }
    Vector2(): Vec2<Type>() { }

    // Destructor
    ~Vector2() { }

    // Accessor methods
    Type GetX(void) const { return this->x_; }
    Type GetY(void) const { return this->y_; }

    // Vector addition
    Vector2 operator+(const Vector2 &vec) const { 
        return Vector2(this->x_ + vec.GetX(), this->y_ + vec.GetY()); 
    }

    // Vector subtraction
    Vector2 operator-(const Vector2 &vec) const { 
        return Vector2(this->x_ - vec.GetX(), this->y_ - vec.GetY()); 
    }

    Vector2 operator+=(const Vector2 &vec) {
        this->x_ += vec.GetX();
        this->y_ += vec.GetY();
        return *this;
    }

    // Dot product
    Type Dot(const Vector2<Type> &vec) const { 
        return this->x_ * vec.GetX() + this->y_ * vec.GetY(); 
    }

    // Magnitude of the vector
    Type Magnitude() const {
        return std::sqrt(this->x_ * this->x_ + this->y_ * this->y_);
    }

    // Normalize the vector
    Vector2 Normalize() const {
        Type mag = Magnitude();
        if (mag > 0) {
            return Vector2(this->x_ / mag, this->y_ / mag);
        }
        return *this;
    }

    // Scalar multiplication
    Vector2 operator*(Type scalar) const {
        return Vector2(this->x_ * scalar, this->y_ * scalar);
    }

    // Scalar division
    Vector2 operator/(Type scalar) const {
        if (scalar != 0) {
            return Vector2(this->x_ / scalar, this->y_ / scalar);
        }
        return *this;
    }

    // Output stream overload
    friend std::ostream& operator<<(std::ostream &os, const Vector2<Type> &vec) {
        os << vec.GetX() << ", " << vec.GetY();
        return os;
    }
};

template <typename Type>
std::ostream& operator<<(std::ostream &os, const Vector2<Type> &vec) {
    os << vec.GetX() << ", " << vec.GetY();
    return os;
}



//////////////////////////////////////////////////////////////////////////////////////
//                     VECTOR3 DOES NOT HAVE ALL OPERATIONS YET                     //
//////////////////////////////////////////////////////////////////////////////////////


template <typename Type>
struct Vec3 {
public:
    Type x_, y_, z_;
    Vec3(): x_(0.0), y_(0.0), z_(0.0) { }
    Vec3(Type x, Type y, Type z): x_(x), y_(y), z_(z) { }
};

template <typename Type>
class Vector3: public Vec3<Type> {
public:
    Vector3(Type x, Type y, Type z): Vec3<Type>(x, y, z) { }
    Vector3(Vector3<Type> &constructorVector): Vec3<Type>(constructorVector.GetX(), constructorVector.GetY(), constructorVector.GetZ()) { }
    Vector3(): Vec3<Type>() { }
    ~Vector3() { }

    Type GetX(void) const { return this->x_; }
    Type GetY(void) const { return this->y_; }
    Type GetZ(void) const { return this->z_; }

    Vector3 operator+(const Vector3 &Vec) const { 
        return Vector3(this->x_ + Vec.GetX(), this->y_ + Vec.GetY(), this->z_ + Vec.GetZ()); 
    }
    Vector3 operator-(const Vector3 &Vec) const { 
        return Vector3(this->x_ - Vec.GetX(), this->y_ - Vec.GetY(), this->z_ - Vec.GetZ()); 
    }
    Type Dot(const Vector3<Type> &Vec) const { 
        return this->x_ * Vec.GetX() + this->y_ * Vec.GetY() + this->z_ * Vec.GetZ(); 
    }

    template <typename T>
    friend std::ostream& operator<<(std::ostream &os, const Vector3<Type> &vec3);
};

template <typename Type>
std::ostream& operator<<(std::ostream &os, const Vector3<Type> &vec3) {
    os << vec3.GetX() << "," << vec3.GetY() << ", " << vec3.GetZ() << std::endl;
    return os;
}
