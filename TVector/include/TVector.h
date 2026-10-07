#pragma once

#include "TMemData.h"

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

// TVector сохраняет идею исходного Vector: элементы лежат в кольцевом буфере.
template <typename T>
class TVector {
protected:
    TMemData<T> _mem;
    std::size_t _front = 0;
    std::size_t _back = 0;

public:
    template <typename Type>
    class Iterator {
        Type* _data = nullptr;
        std::size_t _capacity = 0;
        std::size_t _front = 0;
        std::ptrdiff_t _index = 0;

        Type& current() const {
            if (_capacity == 0) return _data[_index];
            const auto physical = (_front + static_cast<std::size_t>(_index)) % _capacity;
            return _data[physical];
        }

        template <typename>
        friend class Iterator;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::remove_const_t<Type>;
        using difference_type = std::ptrdiff_t;
        using pointer = Type*;
        using reference = Type&;

        Iterator() = default;
        explicit Iterator(Type* pointer) : _data(pointer) {}
        Iterator(Type* data, std::size_t capacity, std::size_t front, std::ptrdiff_t index)
            : _data(data), _capacity(capacity), _front(front), _index(index) {}
        Iterator(const Iterator&) = default;
        Iterator& operator=(const Iterator&) = default;

        template <typename Other,
                  typename = std::enable_if_t<std::is_const_v<Type> &&
                                              std::is_same_v<std::remove_const_t<Type>, Other>>>
        Iterator(const Iterator<Other>& other)
            : _data(other._data), _capacity(other._capacity), _front(other._front), _index(other._index) {}

        bool operator==(const Iterator& other) const noexcept {
            return _data == other._data && _capacity == other._capacity &&
                   _front == other._front && _index == other._index;
        }

        bool operator!=(const Iterator& other) const noexcept { return !(*this == other); }

        Iterator& operator++() { ++_index; return *this; }
        Iterator operator++(int) { Iterator copy(*this); ++(*this); return copy; }
        Iterator& operator--() { --_index; return *this; }
        Iterator operator--(int) { Iterator copy(*this); --(*this); return copy; }

        Iterator operator+(int offset) const { Iterator copy(*this); return copy += offset; }
        Iterator operator-(int offset) const { Iterator copy(*this); return copy -= offset; }
        Iterator& operator+=(int offset) { _index += offset; return *this; }
        Iterator& operator-=(int offset) { _index -= offset; return *this; }

        Type& operator*() { return current(); }
        const Type& operator*() const { return current(); }
        Type* operator->() { return &current(); }
        const Type* operator->() const { return &current(); }
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;

    explicit TVector(std::size_t size = 0) : _mem(size) {
        _back = size == 0 ? 0 : size - 1;
    }

    TVector(std::initializer_list<T> values) : _mem(values) {
        _back = values.size() == 0 ? 0 : values.size() - 1;
    }

    TVector(const T* values, std::size_t size) : _mem(values, size) {
        _back = size == 0 ? 0 : size - 1;
    }

    TVector(const TVector&) = default;
    TVector(TVector&&) noexcept = default;
    virtual ~TVector() = default;
    TVector& operator=(const TVector&) = default;
    TVector& operator=(TVector&&) noexcept = default;

    bool is_empty() const noexcept { return _mem._size == 0; }
    bool is_full() const noexcept { return _mem._size == _mem._capacity; }
    std::size_t get_size() const noexcept { return _mem._size; }
    std::size_t size() const noexcept { return _mem._size; }
    std::size_t get_capacity() const noexcept { return _mem._capacity; }
    std::size_t capacity() const noexcept { return _mem._capacity; }

    T get_front() const { return front(); }
    T get_back() const { return back(); }

    T& front() {
        require_not_empty();
        return (*this)[0];
    }

    const T& front() const {
        require_not_empty();
        return (*this)[0];
    }

    T& back() {
        require_not_empty();
        return (*this)[_mem._size - 1];
    }

    const T& back() const {
        require_not_empty();
        return (*this)[_mem._size - 1];
    }

    T& front_ref() { return front(); }
    T& back_ref() { return back(); }

    T& operator[](std::size_t index) noexcept { return _mem._data[mem_index(index)]; }
    const T& operator[](std::size_t index) const noexcept { return _mem._data[mem_index(index)]; }

    T& at(std::size_t index) {
        check_index(index);
        return (*this)[index];
    }

    const T& at(std::size_t index) const {
        check_index(index);
        return (*this)[index];
    }

    void push_back(const T& value) {
        ensure_capacity(_mem._size + 1);
        _mem._data[mem_index(_mem._size)] = value;
        ++_mem._size;
        _back = mem_index(_mem._size - 1);
    }

    void push_front(const T& value) {
        ensure_capacity(_mem._size + 1);
        _front = (_front + _mem._capacity - 1) % _mem._capacity;
        _mem._data[_front] = value;
        ++_mem._size;
        _back = mem_index(_mem._size - 1);
    }

    void push_back_many(const T* values, std::size_t count) {
        validate_array(values, count);
        for (std::size_t i = 0; i < count; ++i) push_back(values[i]);
    }

    void push_front_many(const T* values, std::size_t count) {
        validate_array(values, count);
        for (std::size_t i = count; i > 0; --i) push_front(values[i - 1]);
    }

    void insert(const T& value, std::size_t position) {
        if (position > _mem._size) throw std::out_of_range("TVector insert position");
        if (position == 0) { push_front(value); return; }
        if (position == _mem._size) { push_back(value); return; }
        ensure_capacity(_mem._size + 1);
        for (std::size_t i = _mem._size; i > position; --i) {
            _mem._data[mem_index(i)] = std::move((*this)[i - 1]);
        }
        _mem._data[mem_index(position)] = value;
        ++_mem._size;
        _back = mem_index(_mem._size - 1);
    }

    void insert_many(const T* values, std::size_t count, std::size_t position) {
        validate_array(values, count);
        if (position > _mem._size) throw std::out_of_range("TVector insert position");
        for (std::size_t i = 0; i < count; ++i) insert(values[i], position + i);
    }

    void pop_back() {
        require_not_empty();
        --_mem._size;
        if (_mem._size == 0) _front = _back = 0;
        else _back = mem_index(_mem._size - 1);
    }

    void pop_front() {
        require_not_empty();
        _front = mem_index(1);
        --_mem._size;
        if (_mem._size == 0) _front = _back = 0;
        else _back = mem_index(_mem._size - 1);
    }

    void pop_back_many(std::size_t count) {
        if (count > _mem._size) throw std::logic_error("Too many elements to remove");
        while (count-- != 0) pop_back();
    }

    void pop_front_many(std::size_t count) {
        if (count > _mem._size) throw std::logic_error("Too many elements to remove");
        while (count-- != 0) pop_front();
    }

    void erase(std::size_t position) {
        check_index(position);
        for (std::size_t i = position; i + 1 < _mem._size; ++i) (*this)[i] = std::move((*this)[i + 1]);
        pop_back();
    }

    void erase_many(std::size_t position, std::size_t count) {
        if (position > _mem._size || count > _mem._size - position) throw std::out_of_range("TVector erase range");
        for (std::size_t i = 0; i < count; ++i) erase(position);
    }

    void clear() noexcept { _mem._size = 0; _front = _back = 0; }

    iterator begin() noexcept { return iterator(_mem._data, _mem._capacity, _front, 0); }
    iterator end() noexcept { return iterator(_mem._data, _mem._capacity, _front, static_cast<std::ptrdiff_t>(_mem._size)); }
    const_iterator begin() const noexcept { return const_iterator(_mem._data, _mem._capacity, _front, 0); }
    const_iterator end() const noexcept { return const_iterator(_mem._data, _mem._capacity, _front, static_cast<std::ptrdiff_t>(_mem._size)); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    // Освобождает всю память, которая не занята элементами.
    void shrink_to_fit() {
        if (_mem._capacity != _mem._size || _front != 0) {
            reallocate(_mem._size);
        }
    }

    friend bool operator==(const TVector& left, const TVector& right) {
        if (left.size() != right.size()) return false;
        for (std::size_t i = 0; i < left.size(); ++i) if (left[i] != right[i]) return false;
        return true;
    }

    friend bool operator!=(const TVector& left, const TVector& right) { return !(left == right); }

    friend std::ostream& operator<<(std::ostream& out, const TVector& vector) {
        out << "{ ";
        for (std::size_t i = 0; i < vector.size(); ++i) {
            if (i != 0) out << ", ";
            out << vector[i];
        }
        return out << " }";
    }

    friend std::istream& operator>>(std::istream& in, TVector& vector) {
        std::size_t size = 0;
        if (!(in >> size)) return in;
        TVector result(size);
        for (std::size_t i = 0; i < size; ++i) {
            if (!(in >> result[i])) { in.setstate(std::ios::failbit); return in; }
        }
        vector = std::move(result);
        return in;
    }

protected:
    std::size_t mem_index(std::size_t index) const noexcept { return (_front + index) % _mem._capacity; }

    void ensure_capacity(std::size_t required) {
        if (required > _mem._capacity) reallocate(TMemData<T>::calculate_capacity(required));
    }

    void reallocate(std::size_t new_capacity) {
        T* replacement = new_capacity == 0 ? nullptr : new T[new_capacity]{};
        for (std::size_t i = 0; i < _mem._size; ++i) replacement[i] = std::move((*this)[i]);
        delete[] _mem._data;
        _mem._data = replacement;
        _mem._capacity = new_capacity;
        _front = 0;
        _back = _mem._size == 0 ? 0 : _mem._size - 1;
    }

private:
    void require_not_empty() const {
        if (is_empty()) throw std::logic_error("TVector is empty");
    }

    void check_index(std::size_t index) const {
        if (index >= _mem._size) throw std::out_of_range("TVector index");
    }

    static void validate_array(const T* values, std::size_t count) {
        if (count != 0 && values == nullptr) throw std::invalid_argument("Null source array");
    }
};

// Совместимость с именем класса в приложенном архиве.
template <typename T>
using Vector = TVector<T>;
