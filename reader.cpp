#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

#include "common.h"
#include "ring_buffer.h"

uint32_t diff_timespec(std::timespec const& t1, std::timespec const& t2) {
    return (t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec);
}

int main() {
    set_affinity(3);

    int fd;
    ring_buffer* buffer_ptr = open_shared_memory(fd);

    std::timespec readed, current;
    uint32_t diffs[OPERATIONS_CNT];

    for (int i = 0; i < OPERATIONS_CNT; ++i) {
        while (!buffer_ptr->read(readed)) {
        }
        clock_gettime(CLOCK_REALTIME, &current);
        diffs[i] = diff_timespec(readed, current);
    }
    close_shared_memory(fd, buffer_ptr);

    for (auto& elem : diffs) {
        std::cout << elem << "\n";
    }
}
