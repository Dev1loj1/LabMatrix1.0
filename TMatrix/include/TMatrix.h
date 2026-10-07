#pragma once

#include "TMathVector.h"

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <ostream>
#include <stdexcept>

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

template <typename T>
TMatrix<T>::TMatrix() = default;

template <typename T>
TMatrix<T>::TMatrix(std::size_t size) : Base(size) {
    for (std::size_t row = 0; row < size; ++row) (*this)[row] = Row(size);
}

template <typename T>
TMatrix<T>::TMatrix(std::initializer_list<std::initializer_list<T>> rows) : Base(rows.size()) {
    const std::size_t size = rows.size();
    std::size_t row = 0;
    for (const auto& values : rows) {
        if (values.size() != size) throw std::invalid_argument("TMatrix must be square");
        (*this)[row++] = Row(values);
    }
}

template <typename T>
std::size_t TMatrix<T>::order() const noexcept {
    return this->size();
}

template <typename T>
T& TMatrix<T>::at(std::size_t row, std::size_t column) {
    return Base::at(row).at(column);
}

template <typename T>
const T& TMatrix<T>::at(std::size_t row, std::size_t column) const {
    return Base::at(row).at(column);
}

template <typename T>
TMatrix<T> TMatrix<T>::operator+(const TMatrix& other) const {
    TMatrix result(*this);
    return result += other;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator-(const TMatrix& other) const {
    TMatrix result(*this);
    return result -= other;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const TMatrix& other) const {
    require_same_order(other);
    TMatrix result(order());
    for (std::size_t row = 0; row < order(); ++row) {
        for (std::size_t column = 0; column < order(); ++column) {
            T value{};
            for (std::size_t index = 0; index < order(); ++index) {
                value += (*this)[row][index] * other[index][column];
            }
            result[row][column] = value;
        }
    }
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::operator*(const T& scalar) const {
    TMatrix result(*this);
    return result *= scalar;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator+=(const TMatrix& other) {
    require_same_order(other);
    for (std::size_t row = 0; row < order(); ++row) (*this)[row] += other[row];
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator-=(const TMatrix& other) {
    require_same_order(other);
    for (std::size_t row = 0; row < order(); ++row) (*this)[row] -= other[row];
    return *this;
}

template <typename T>
TMatrix<T>& TMatrix<T>::operator*=(const T& scalar) {
    for (std::size_t row = 0; row < order(); ++row) (*this)[row] *= scalar;
    return *this;
}

template <typename T>
TMatrix<T> TMatrix<T>::transpose() const {
    TMatrix result(order());
    for (std::size_t row = 0; row < order(); ++row) {
        for (std::size_t column = 0; column < order(); ++column) result[column][row] = (*this)[row][column];
    }
    return result;
}

template <typename T>
T TMatrix<T>::trace() const {
    T result{};
    for (std::size_t index = 0; index < order(); ++index) result += (*this)[index][index];
    return result;
}

template <typename T>
TMatrix<T> TMatrix<T>::identity(std::size_t size) {
    TMatrix result(size);
    for (std::size_t index = 0; index < size; ++index) result[index][index] = T{1};
    return result;
}

template <typename T>
void TMatrix<T>::require_same_order(const TMatrix& other) const {
    if (order() != other.order()) throw std::invalid_argument("Different matrix orders");
}

template <typename T>
TMatrix<T> operator*(const T& scalar, const TMatrix<T>& matrix) {
    return matrix * scalar;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const TMatrix<T>& matrix) {
    for (std::size_t row = 0; row < matrix.order(); ++row) {
        out << "[ ";
        for (std::size_t column = 0; column < matrix.order(); ++column) {
            out << matrix[row][column] << (column + 1 == matrix.order() ? "" : " ");
        }
        out << " ]";
        if (row + 1 != matrix.order()) out << '\n';
    }
    return out;
}
