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

	struct io_open *io = calloc(1, sizeof(*io)); 
	if (!io)
		return NULL;

	io->art_io.ops = &io_open_ops;
	io->fd = open(uri->parameters[0], O_RDWR | O_APPEND | O_CREAT, 0600);
	if (io->fd < 0)
		goto free_io;

	return (struct lart_io *)io;

free_io:
	free(io);
	return NULL;
}

static int io_close(struct lart_io *art_io)
{
	struct io_open *io = container_of(art_io, struct io_open, art_io);
	int ret = close(io->fd);
	free(io);
	return ret;
}

static ssize_t io_write(struct lart_io *art_io, const void *buf, size_t count)
{
	struct io_open *io = container_of(art_io, struct io_open, art_io);
	return write(io->fd, buf, count);
}

static ssize_t io_read(struct lart_io *art_io, void *buf, size_t count)
{
	struct io_open *io = container_of(art_io, struct io_open, art_io);
	return read(io->fd, buf, count);
}

const static struct lart_io_ops io_open_ops = {
	.address_type = "OPEN",

	.open = io_open,
	.close = io_close,

	.write = io_write,
	.read = io_read,
};

REGISTER_IO_CONTROLLER(io_open_ops);
