#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

inline constexpr std::size_t MEM_STEP = 15;

template <typename T>
class TVector;

// Динамическое хранилище из исходного класса MemData.
template <typename T>
class TMemData {
    T* _data = nullptr;
    std::size_t _size = 0;
    std::size_t _capacity = 0;

public:
    explicit TMemData(std::size_t size = 0) { allocate_for(size); }

    TMemData(std::initializer_list<T> values) {
        allocate_for(values.size());
        std::copy(values.begin(), values.end(), _data);
    }

    TMemData(const T* values, std::size_t size) {
        if (size != 0 && values == nullptr) {
            throw std::invalid_argument("Null source array");
        }
        allocate_for(size);
        std::copy(values, values + size, _data);
    }

    TMemData(const TMemData& other) {
        allocate_exact(other._capacity, other._size);
        std::copy(other._data, other._data + _size, _data);
    }

    TMemData(TMemData&& other) noexcept
        : _data(other._data), _size(other._size), _capacity(other._capacity) {
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    ~TMemData() { delete[] _data; }

    TMemData& operator=(const TMemData& other) {
        if (this != &other) {
            TMemData copy(other);
            swap(copy);
        }
        return *this;
    }

    TMemData& operator=(TMemData&& other) noexcept {
        if (this != &other) {
            delete[] _data;
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            other._data = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;
    }

    bool operator==(const TMemData& other) const {
        if (_size != other._size) {
            return false;
        }
        for (std::size_t i = 0; i < _size; ++i) {
            if (_data[i] != other._data[i]) {
                return false;
            }
        }
        return true;
    }

    bool is_empty() const noexcept { return _size == 0; }
    std::size_t get_size() const noexcept { return _size; }
    std::size_t get_capacity() const noexcept { return _capacity; }
    const T* get_data_const() const noexcept { return _data; }
    T* get_data_changeable() noexcept { return _data; }

    void clear_memory() {
        delete[] _data;
        _data = nullptr;
        allocate_for(0);
    }

private:
    static std::size_t calculate_capacity(std::size_t size) {
        return size == 0 ? MEM_STEP : ((size - 1) / MEM_STEP + 1) * MEM_STEP;
    }

    void allocate_for(std::size_t size) {
        allocate_exact(calculate_capacity(size), size);
    }

    void allocate_exact(std::size_t capacity, std::size_t size) {
        _data = capacity == 0 ? nullptr : new T[capacity]{};
        _capacity = capacity;
        _size = size;
    }

    void swap(TMemData& other) noexcept {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    friend class TVector<T>;
};
