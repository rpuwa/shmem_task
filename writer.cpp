#include <ctime>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

#include "common.h"
#include "ring_buffer.h"

int main(int argc, char* argv[]) {
    set_affinity(1);
    if (argc != 2) {
        throw std::runtime_error("Invalid arguments passed. Usage: ./writer <time-to-sleep>");
    }
    uint32_t const WRITER_TIMESTAMP_NSEC = atoi(argv[1]);

    int fd;
    ring_buffer* buffer_ptr = open_shared_memory(fd);

    std::timespec to_write, to_sleep;
    to_sleep.tv_sec = 0;
    to_sleep.tv_nsec = WRITER_TIMESTAMP_NSEC;
    for (int i = 0; i < OPERATIONS_CNT; ++i) {
        clock_gettime(CLOCK_REALTIME, &to_write);
        while (!buffer_ptr->write(to_write)) {
        }
        if (WRITER_TIMESTAMP_NSEC) {
            clock_nanosleep(CLOCK_REALTIME, 0, &to_sleep, NULL);
        }
    }
    close_shared_memory(fd, buffer_ptr);
}
