#pragma once

#include "TVector.h"

#include <cstddef>

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
