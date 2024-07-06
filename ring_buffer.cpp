#include "ring_buffer.h"

bool ring_buffer::write(std::timespec &item) {
    if (full()) {
        return false;
    }
    size_t next_head = head_ + 1;
    if (next_head == capacity()) {
        next_head = 0;
    }
    if (next_head == tail_) {
        full_ = true;
    }
    std::swap(buf_[head_], item);
    head_ = next_head;
    return true;
}

bool ring_buffer::read(std::timespec &res) {
    if (empty()) {
        return false;
    }
    size_t next_tail = tail_ + 1;
    if (next_tail == capacity()) {
        next_tail = 0;
    }
    std::swap(buf_[tail_], res);
    tail_ = next_tail;
    full_ = false;
    return true;
}

bool ring_buffer::empty() const {
    return !full_ && head_ == tail_;
}

bool ring_buffer::full() const {
    return full_;
}
