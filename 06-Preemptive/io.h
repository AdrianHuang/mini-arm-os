#ifndef IO_H
#define IO_H

enum open_types_t {
    O_RDONLY = 0,
    O_WRONLY = 1,
    O_RDWR = 2,
    O_CREAT = 4,
    O_TRUNC = 8,
    O_APPEND = 16,
};

size_t write(int fd, const void *buf, size_t bytes);
int open(const char *pathname, int flags);
int close(int fd);
#endif
