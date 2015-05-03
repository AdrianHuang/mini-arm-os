#include <stdint.h>
#include "host.h"

size_t write(int fd, const void *buf, size_t bytes)
{
	return host_action(SYS_WRITE, fd, (void *) buf, bytes);
}

int open(const char *pathname, int flags)
{
	return host_action(SYS_OPEN, pathname, flags);
}

int close(int fd)
{
	return host_action(SYS_CLOSE, fd);
}
