#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <random>
#include <stdexcept>
#include <utility>

inline constexpr std::size_t MEM_STEP = 15;

inline std::size_t calculate_capacity(std::size_t size) {
    return (size / MEM_STEP + 1) * MEM_STEP;
}

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
        if (size != 0) std::copy(values, values + size, _data);
    }

    TMemData(const TMemData& other) {
        allocate_exact(other._capacity, other._size);
        if (_size != 0) std::copy(other._data, other._data + _size, _data);
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

    void set_memory(std::size_t size) {
        delete[] _data;
        _data = nullptr;
        allocate_for(size);
    }

    void reset_memory(std::size_t size, std::size_t start_index = 0) {
        if (_capacity != 0 && start_index >= _capacity) throw std::out_of_range("TMemData start index");
        const std::size_t new_capacity = calculate_capacity(size);
        if (new_capacity == _capacity && start_index == 0) {
            _size = size;
            return;
        }
        T* replacement = new T[new_capacity]{};
        const std::size_t copy_count = std::min(_size, size);
        for (std::size_t i = 0; i < copy_count; ++i) {
            replacement[i] = _data[(start_index + i) % _capacity];
        }
        delete[] _data;
        _data = replacement;
        _capacity = new_capacity;
        _size = size;
    }

    void set_size(std::size_t size) {
        if (size > _capacity) throw std::invalid_argument("Size is greater than capacity");
        _size = size;
    }

    void clear_memory() {
        delete[] _data;
        _data = nullptr;
        allocate_for(0);
    }

private:
    void allocate_for(std::size_t size) {
        allocate_exact(::calculate_capacity(size), size);
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

template <typename T>
using MemData = TMemData<T>;

template <typename T>
void quick_sort(TMemData<T>& memory) {
    std::sort(memory.get_data_changeable(), memory.get_data_changeable() + memory.get_size());
}

template <typename T>
void shuffle(TMemData<T>& memory) {
    static std::mt19937 generator(std::random_device{}());
    std::shuffle(memory.get_data_changeable(), memory.get_data_changeable() + memory.get_size(), generator);
}
