#pragma once
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "ring_buffer.h"

#define SHARED_MEMORY_NAME "/shmem"
#define OPERATIONS_CNT 1000000

void set_affinity(int num_cpu) {
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    CPU_SET(num_cpu, &cpu_set);

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set)) {
        throw std::runtime_error("Operation pthread_setaffinity_np failed");
    }
    if (pthread_getaffinity_np(pthread_self(), sizeof(cpu_set), &cpu_set)) {
        throw std::runtime_error("Operation pthread_getaffinity_np failed");
    }
}

ring_buffer* open_shared_memory(int& fd) {
    fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        throw std::runtime_error("Operation shm_open failed");
    }
    if (ftruncate(fd, RING_BUFFER_SIZE)) {
        throw std::runtime_error("Operation ftruncate failed");
    }

    ring_buffer* buffer_ptr =
            (ring_buffer*)mmap(0, RING_BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer_ptr == MAP_FAILED) {
        throw std::runtime_error("Operation mmap failed");
    }
    return buffer_ptr;
}

void close_shared_memory(int& fd, ring_buffer* buffer_ptr) {
    munmap(buffer_ptr, RING_BUFFER_SIZE);
    shm_unlink(SHARED_MEMORY_NAME);
    close(fd);
}