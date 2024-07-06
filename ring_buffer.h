#pragma once

#include <atomic>
#include <ctime>

// ring_buffer size will fit into 2KB
#define RING_BUF_SIZE 126

class ring_buffer {
public:
    ring_buffer() noexcept {}

    /*
     Write a new value to the head of the buffer.
     @param item A reference to `struct timespec` which stores the value we want to write.
     @return `false` if buffer is full, otherwise `true`.
    */
    bool write(std::timespec &item);
    /*
     Read the value from the tail of the buffer.
     @param res A reference to `struct timespec` in which the result will be written.
     @return `false` if buffer is empty, otherwise `true`.
    */
    bool read(std::timespec &res);

private:
    std::timespec buf_[RING_BUF_SIZE];
    std::atomic<size_t> head_ = 0;
    std::atomic<size_t> tail_ = 0;
    std::atomic<bool> full_ = false;

    static size_t capacity() {
        return RING_BUF_SIZE;
    }

    inline bool empty() const;
    inline bool full() const;
};

size_t const RING_BUFFER_SIZE = sizeof(ring_buffer);
