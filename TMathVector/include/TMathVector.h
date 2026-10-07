#pragma once

#include "TVector.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

template <typename T>
class TMathVector : public TVector<T> {
    using Base = TVector<T>;

public:
    using Base::Base;

    TMathVector() = default;
    TMathVector(const Base& vector);
    TMathVector(Base&& vector) noexcept;

    TMathVector operator+(const TMathVector& other) const;
    TMathVector operator-(const TMathVector& other) const;
    TMathVector& operator+=(const TMathVector& other);
    TMathVector& operator-=(const TMathVector& other);

    TMathVector operator*(const T& scalar) const;
    TMathVector operator/(const T& scalar) const;
    TMathVector& operator*=(const T& scalar);
    TMathVector& operator/=(const T& scalar);

    T operator*(const TMathVector& other) const;
    T dot(const TMathVector& other) const;
    double length() const;

private:
    void require_same_size(const TMathVector& other) const;
};

template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& vector);

template <typename T>
TMathVector<T>::TMathVector(const Base& vector) : Base(vector) {}

template <typename T>
TMathVector<T>::TMathVector(Base&& vector) noexcept : Base(std::move(vector)) {}

template <typename T>
TMathVector<T> TMathVector<T>::operator+(const TMathVector& other) const {
    TMathVector result(*this);
    return result += other;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator-(const TMathVector& other) const {
    TMathVector result(*this);
    return result -= other;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator+=(const TMathVector& other) {
    require_same_size(other);
    for (std::size_t i = 0; i < this->size(); ++i) (*this)[i] += other[i];
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator-=(const TMathVector& other) {
    require_same_size(other);
    for (std::size_t i = 0; i < this->size(); ++i) (*this)[i] -= other[i];
    return *this;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator*(const T& scalar) const {
    TMathVector result(*this);
    return result *= scalar;
}

template <typename T>
TMathVector<T> TMathVector<T>::operator/(const T& scalar) const {
    TMathVector result(*this);
    return result /= scalar;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator*=(const T& scalar) {
    for (std::size_t i = 0; i < this->size(); ++i) (*this)[i] *= scalar;
    return *this;
}

template <typename T>
TMathVector<T>& TMathVector<T>::operator/=(const T& scalar) {
    if (scalar == T{}) throw std::invalid_argument("Division by zero");
    for (std::size_t i = 0; i < this->size(); ++i) (*this)[i] /= scalar;
    return *this;
}

template <typename T>
T TMathVector<T>::operator*(const TMathVector& other) const {
    return dot(other);
}

template <typename T>
T TMathVector<T>::dot(const TMathVector& other) const {
    require_same_size(other);
    T result{};
    for (std::size_t i = 0; i < this->size(); ++i) result += (*this)[i] * other[i];
    return result;
}

template <typename T>
double TMathVector<T>::length() const {
    long double square_sum = 0.0L;
    for (std::size_t i = 0; i < this->size(); ++i) {
        const long double value = static_cast<long double>((*this)[i]);
        square_sum += value * value;
    }
    return static_cast<double>(std::sqrt(square_sum));
}

template <typename T>
void TMathVector<T>::require_same_size(const TMathVector& other) const {
    if (this->size() != other.size()) throw std::invalid_argument("Different vector sizes");
}

template <typename T>
TMathVector<T> operator*(const T& scalar, const TMathVector<T>& vector) {
    return vector * scalar;
}
