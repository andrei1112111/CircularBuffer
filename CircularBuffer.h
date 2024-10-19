#pragma once
#ifndef CIRCULARBUFFER_CIRCULARBUFFER_H
#define CIRCULARBUFFER_CIRCULARBUFFER_H

#include <stdexcept>
#include <algorithm>


template<typename value_type>
class CircularBuffer {
private:
    value_type *buffer;  // Pointer to the buffer that holds the elements

    int begin;           // Index of the first element in the buffer
    int end;             // Index of the position to insert the next element

    int buffer_size;     // Current number of elements in the buffer
    int buffer_capacity; // Maximum capacity of the buffer

public:
    // Constructor: Creates an empty circular buffer with zero capacity
    CircularBuffer() {
        this->buffer = new value_type[0];
        this->buffer_size = 0;
        this->buffer_capacity = 0;
        this->end = 0;
        this->begin = 0;
    }

    // Destructor: Cleans up allocated memory for the buffer
    ~CircularBuffer() {
        if (this->buffer != nullptr && this->buffer_capacity > 0) {
            delete[] this->buffer;
            this->buffer_size = 0;
            this->buffer_capacity = 0;
            this->end = 0;
            this->begin = 0;
        }
    }

    // Copy constructor: Creates a new circular buffer as a copy of another buffer
    CircularBuffer(const CircularBuffer &cb) {
        this->buffer = new value_type[cb.buffer_capacity];
        this->buffer_size = cb.buffer_size;
        this->buffer_capacity = cb.buffer_capacity;
        this->end = cb.end;
        this->begin = cb.begin;
        if (cb.buffer && this->buffer) {
            for (int i = 0; i < cb.buffer_capacity; ++i) {
                this->buffer[i] = cb.buffer[i];
            }
        }
    }

    // Constructor: Creates a circular buffer with a specified capacity
    explicit CircularBuffer(int capacity) { // запрещает брать входной аргумент другого типа, делая неявное преобразование
        if (capacity < 0) { capacity = 0; }
        this->buffer = new value_type[capacity];
        this->buffer_size = 0;
        this->buffer_capacity = capacity;
        this->begin = 0;
        this->end = 0;
    }

    // Constructor: Creates a circular buffer with a specified capacity and initializes all elements with a given value
    CircularBuffer(int capacity, const value_type &elem) {
        if (capacity < 0) { capacity = 0; }
        this->buffer = new value_type[capacity];
        this->buffer_size = capacity;
        this->buffer_capacity = capacity;
        this->begin = 0;
        this->end = 0;

        for (int i = 0; i < capacity; ++i) {
            this->buffer[i] = elem;  // Initialize all elements with elem
        }
    }

    // Access operator: Provides direct access to the element at the specified index, wrapping around the buffer
    value_type &operator[](int i) {
        return this->buffer[i % this->size()];
    }

    // Const access operator: Provides read-only access to the element at the specified index, wrapping around the buffer
    const value_type &operator[](int i) const {
        return this->buffer[i % this->size()];
    }

    // Access method: Returns a reference to the element at the specified index, throws if index is out of range
    value_type &at(int i) {
        if (i % this->size() < this->size()) {
            return this->buffer[i % this->size()];
        }
        throw std::invalid_argument("The index is not from a filled circular buffer");
    }

    // Const access method: Returns a read-only reference to the element at the specified index, throws if index is out of range
    [[nodiscard]] const value_type &at(int i) const {
        if (i % this->size() < this->size()) {
            return this->buffer[i % this->size()];
        }
        throw std::invalid_argument("The index is not from a filled circular buffer");
    }

    // Method: Returns a reference to the first element in the buffer, throws if the buffer is empty
    value_type &front() {
        if (this->size() == 0) { throw std::out_of_range("buffer is empty"); }
        return this->buffer[begin];
    }

    // Method: Returns a reference to the last element in the buffer, throws if the buffer is empty
    value_type &back() {
        if (this->size() == 0) { throw std::out_of_range("buffer is empty"); }
        if (this->end - 1 < 0) { return this->buffer[this->buffer_size - 1]; }
        return this->buffer[this->end - 1];
    }

    // Const method: Returns a read-only reference to the first element in the buffer, throws if the buffer is empty
    const value_type &front() const {
        if (this->size() == 0) { throw std::out_of_range("buffer is empty"); }
        return this->buffer[begin];
    }

    // Const method: Returns a read-only reference to the last element in the buffer, throws if the buffer is empty
    [[nodiscard]] const value_type &back() const {
        if (this->size() == 0) { throw std::out_of_range("buffer is empty"); }
        if (this->end - 1 < 0) { return this->buffer[this->buffer_size - 1]; }
        return this->buffer[this->end - 1];
    }

    // Method: Linearizes the buffer so that the first element moves to the start of the allocated memory,
    //         returns a pointer to the first element.
    value_type *linearize() {
        if (this->begin == 0) return this->buffer;
        auto *new_buffer = new value_type[this->buffer_capacity];

        for (int i = 0; i < this->buffer_size; ++i) {
            new_buffer[i] = this->buffer[(this->begin + i) % this->buffer_capacity];
        }

        delete[] this->buffer;
        this->buffer = new_buffer;
        this->begin = 0;
        this->end = this->buffer_size;

        return this->buffer;
    }

    // Method: Checks if the buffer is linearized (i.e., if the first element is at index 0).
    [[nodiscard]] bool is_linearized() const {
        return this->begin == 0;
    }

    // Method: Rotates the buffer so that the element at the new_begin index becomes the first element.
    void rotate(int new_begin) {
        if (new_begin < 0 || new_begin >= this->buffer_size) {
            throw std::out_of_range("new_begin index out of range");
        }
        this->begin = (this->begin + new_begin) % this->buffer_capacity;
        this->end = this->begin + this->buffer_size; // Update end based on new begin
    }

    // Method: Returns the current number of elements in the buffer
    [[nodiscard]] int size() const { return this->buffer_size; }

    // Method: Checks if the buffer is empty
    [[nodiscard]] bool empty() const { return this->buffer_size == 0; }

    // Method: Checks if the buffer is full
    [[nodiscard]] bool full() const { return this->buffer_size == this->buffer_capacity; }

    // Method: Returns the number of elements that can be added before the buffer is full
    [[nodiscard]] int reserve() const { return this->capacity() - this->size(); }

    // Method: Returns the maximum capacity of the buffer
    [[nodiscard]] int capacity() const { return buffer_capacity; }

    // Method: Sets a new capacity for the buffer, reallocating if necessary
    void set_capacity(int new_capacity) {
        if (new_capacity < 0) {
            throw std::out_of_range("Capacity must be non-negative");
        }
        if (new_capacity == this->buffer_capacity) {
            return; // No change needed
        }

        auto *new_buffer = new value_type[new_capacity];

        int num_elements_to_copy = std::min(this->buffer_size, new_capacity);

        for (int i = 0; i < num_elements_to_copy; ++i) {
            new_buffer[i] = this->buffer[(this->begin + i) % this->buffer_capacity];
        }

        delete[] this->buffer;
        this->buffer = new_buffer;
        this->buffer_capacity = new_capacity;

        this->buffer_size = num_elements_to_copy;

        this->begin = 0;
        this->end = num_elements_to_copy;
    }

    // Method: Resizes the buffer to a new size. If the new size is greater than the current size,
    //         new elements will be initialized with the specified item.
    void resize(int new_size, const value_type &item = value_type()) {
        if (new_size > this->capacity() && new_size >= 0) {
            throw std::out_of_range("must be 0 <= new_size <= circular buffer capacity");
        }
        if (new_size == this->size()) { return; }
        if (new_size < this->size()) {
            for (int i = 0; i < this->size() - new_size; ++i) {
                this->pop_back(); // Remove elements from the back if resizing down
            }
        }
        if (new_size > this->size()) {
            for (int i = 0; i < new_size - this->size(); ++i) {
                this->push_back(item); // Add new elements with the specified item if resizing up
            }
        }
    }

    // Assignment operator: Copies the content from another circular buffer to this one
    CircularBuffer &operator=(const CircularBuffer &cb) {
        if (this == &cb) { return *this; }

        this->buffer = new value_type[cb.buffer_capacity];
        this->buffer_size = cb.buffer_size;
        this->buffer_capacity = cb.buffer_capacity;
        this->end = cb.end;
        this->begin = cb.begin;
        if (cb.buffer && this->buffer) {
            for (int i = 0; i < cb.buffer_capacity; ++i) {
                this->buffer[i] = cb.buffer[i];
            }
        }

        return *this;
    }

    // Method: Swaps the contents of this buffer with another buffer, requires equal capacity
    void swap(CircularBuffer &cb) {
        if (this->buffer_capacity == cb.buffer_capacity) {
            for (int i = 0; i < cb.buffer_capacity; ++i) {
                std::swap(this->buffer[i], cb.buffer[i]);
            }
            std::swap(this->buffer_size, cb.buffer_size);
            std::swap(this->end, cb.end);
            std::swap(this->begin, cb.begin);
        } else {
            throw std::invalid_argument("Can't swap not equal capacity buffers");
        }
    }

    // Method: Adds a new element to the back of the buffer, overwriting the front element if the buffer is full
    void push_back(const value_type &item = value_type()) {
        if (this->full()) {  // Rewrite front element if full
            this->buffer[this->end++] = item;
            ++this->begin;
            if (this->begin == this->buffer_capacity) { this->begin = 0; }
        } else {
            this->buffer[this->end++] = item;
            ++this->buffer_size;
        }
        if (this->end == this->buffer_capacity) { this->end = 0; }
    }

    // Method: Adds a new element to the front of the buffer, overwriting the back element if the buffer is full
    void push_front(const value_type &item = value_type()) {
        if (!(this->full())) {  // Not overwrite back element
            this->buffer_size += 1;
        }
        --begin;
        if (begin < 0) { begin = this->capacity() - 1; }
        this->buffer[begin] = item;
    }

    // Method: Removes the last element from the buffer, throws if the buffer is empty
    void pop_back() {
        if (this->empty()) {
            throw std::out_of_range("there is no items in buffer");
        } else {
            --this->end;
            --this->buffer_size;
            if (this->end < 0) { this->end = this->capacity() - 1; }
            if (this->buffer != nullptr) { this->buffer[this->end] = {}; }
        }
    }

    // Method: Removes the first element from the buffer, throws if the buffer is empty
    void pop_front() {
        if (begin == end) {
            throw std::out_of_range("there is no items in buffer");
        } else {
            this->buffer[this->begin] = {};
            ++this->begin;
            --this->buffer_size;
            if (this->begin >= this->capacity()) { this->begin = 0; }
        }
    }

    // Method: Inserts a new element at the specified position, shifts elements as necessary,
    //         may overwrite the front element if the buffer is full
    void insert(int pos, const value_type &item = value_type()) {
        if (pos < 0 || pos > this->size()) {
            throw std::out_of_range("Position out of range");
        }
        if (this->full()) {
            pop_front(); // Remove front element if buffer is full
        }
        for (int i = this->size(); i > pos; --i) {
            this->buffer[(this->begin + i) % this->buffer_capacity] = this->buffer[(this->begin + i - 1) %
                                                                                   this->buffer_capacity];
        }
        this->buffer[(this->begin + pos) % this->buffer_capacity] = item;
        ++this->buffer_size;
    }

    // Method: Removes elements from the buffer in the specified range [first, last)
    void erase(int first, int last) {
        if (first < 0 || last > this->size() || first > last) {
            throw std::out_of_range("Invalid range for erase");
        }
        for (int i = last; i < this->size(); ++i) {
            this->buffer[(this->begin + i - (last - first)) % this->buffer_capacity] = this->buffer[(this->begin + i) %
                                                                                                    this->buffer_capacity];
        }
        this->buffer_size -= (last - first);
        this->end -= (last - first);
    }

    // Method: Clears the buffer, resetting size and positions
    void clear() {
        this->buffer_size = 0;
        this->begin = 0;
        this->end = 0;
        if (this->buffer) {
            for (int i = 0; i < this->capacity(); ++i) { this->buffer[i] = {}; }
        }
    }
};

// Checks if two circular buffers are not equal
template<class T>
bool operator!=(const CircularBuffer<T> &a, const CircularBuffer<T> &b) {
    return !(a == b);
}

// Checks if two circular buffers are equal
template<class T>
bool operator==(const CircularBuffer<T> &a, const CircularBuffer<T> &b) {
    if (a.size() != b.size()) { return false; }
    if (a.capacity() != b.capacity()) { return false; }
    if (a.capacity() > 0) {
        if (a.front() != b.front()) { return false; }
        if (a.back() != b.back()) { return false; }
        for (int i = 0; i < a.capacity(); ++i) {
            if (a[i] != b[i]) { return false; }
        }
    }
    return true;
}

#endif //CIRCULARBUFFER_CIRCULARBUFFER_H
