#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>
#include <iosfwd>

// Квадратная матрица по условию лабораторной работы.
template <typename T>
class TMatrix : public TMathVector<TMathVector<T>> {
    using Row = TMathVector<T>;
    using Base = TMathVector<Row>;

public:
    TMatrix();
    explicit TMatrix(std::size_t size);
    TMatrix(std::initializer_list<std::initializer_list<T>> rows);

    std::size_t order() const noexcept;
    T& at(std::size_t row, std::size_t column);
    const T& at(std::size_t row, std::size_t column) const;

    TMatrix operator+(const TMatrix& other) const;
    TMatrix operator-(const TMatrix& other) const;
    TMatrix operator*(const TMatrix& other) const;
    TMatrix operator*(const T& scalar) const;
    TMatrix& operator+=(const TMatrix& other);
    TMatrix& operator-=(const TMatrix& other);
    TMatrix& operator*=(const T& scalar);

    TMatrix transpose() const;
    T trace() const;
    static TMatrix identity(std::size_t size);

    template <typename U>
    friend TMatrix<U> operator*(const U& scalar, const TMatrix<U>& matrix);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& out, const TMatrix<U>& matrix);

private:
    void require_same_order(const TMatrix& other) const;
};
