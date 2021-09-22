#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "io_controllers.h"

struct io_open {
	struct lart_io art_io;
	int fd;
};

const static struct lart_io_ops io_open_ops;

static struct lart_io *io_open(struct lart_io_uri *uri)
{
	errno = EINVAL;
	if (!uri) 
		return NULL;

	struct io_open *io_open = calloc(1, sizeof(*io_open)); 
	if (!io_open)
		return NULL;

	io_open->art_io.ops = &io_open_ops;
	io_open->fd = open(uri->parameters[0], O_RDWR | O_APPEND | O_CREAT, 0600);

	return (struct lart_io *)io_open;
}

static int io_close(struct lart_io *io)
{
}

static ssize_t io_write(struct lart_io *io, const void *buf, size_t count)
{
}

static ssize_t io_read(struct lart_io *io, void *buf, size_t count)
{
}

const static struct lart_io_ops io_open_ops = {
	.address_type = "OPEN",

	.open = io_open,
	.close = io_close,

	.write = io_write,
	.read = io_read,
};

REGISTER_IO_CONTROLLER(io_open_ops);
