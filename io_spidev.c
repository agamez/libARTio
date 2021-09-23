#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <sys/ioctl.h>

#include "io_controllers.h"

struct io_spidev {
	struct lart_io art_io;
	int fd;
	int bits_per_word;
};

const static struct lart_io_ops io_spidev_ops;

static struct lart_io *io_open(struct lart_io_uri *uri)
{
	errno = EINVAL;
	if (!uri) 
		return NULL;

	struct io_spidev *io = calloc(1, sizeof(*io)); 
	if (!io)
		return NULL;

	io->art_io.ops = &io_spidev_ops;
	io->fd = open(uri->parameters[0], O_RDWR | O_APPEND | O_CREAT, 0600);
	if (io->fd < 0)
		goto free_io;

	int ret, var, var_read;
	for (int i = 0; i < LART_IO_URI_MAX_OPTIONS && uri->options[i]; i++) {
		switch (uri->options[i][0]) {
		case 'm':
			switch (uri->options[i][1]) {
				case '0': var = SPI_MODE_0; break;
				case '1': var = SPI_MODE_1; break;
				case '2': var = SPI_MODE_2; break;
				case '3': var = SPI_MODE_3; break;
				default:
					goto close_fd;
			}
			ioctl(io->fd, SPI_IOC_WR_MODE, &var);
			ret = ioctl(io->fd, SPI_IOC_RD_MODE, &var_read);
			if (ret < 0 || var != var_read)
				goto close_fd;

			break;

		case 'M':
			var = 0;
			ioctl(io->fd, SPI_IOC_WR_LSB_FIRST, &var);
			ret = ioctl(io->fd, SPI_IOC_RD_LSB_FIRST, &var_read);
			if (ret < 0 || var != 0)
				goto close_fd;

			break;

		case 'L':
			var = 1;
			ioctl(io->fd, SPI_IOC_WR_LSB_FIRST, &var);
			ret = ioctl(io->fd, SPI_IOC_RD_LSB_FIRST, &var_read);
			if (ret < 0 || var != 0)
				goto close_fd;

			break;

		case 0 ... 9:
			var = atoi(uri->options[i]);
			ioctl(io->fd, SPI_IOC_WR_BITS_PER_WORD, &var);
			ret = ioctl(io->fd, SPI_IOC_RD_BITS_PER_WORD, &var_read);
			if (ret < 0 || var != var_read)
				goto close_fd;

			io->bits_per_word = var;
			break;
		}
	}

	return (struct lart_io *)io;

close_fd:
	close(io->fd);

free_io:
	free(io);
	return NULL;
}

static int io_close(struct lart_io *art_io)
{
	struct io_spidev *io = container_of(art_io, struct io_spidev, art_io);
	int ret = close(io->fd);
	free(io);
	return ret;
}

static ssize_t io_write(struct lart_io *art_io, const void *buf, size_t count)
{
	struct io_spidev *io = container_of(art_io, struct io_spidev, art_io);
	return write(io->fd, buf, count);
}

static ssize_t io_read(struct lart_io *art_io, void *buf, size_t count)
{
	struct io_spidev *io = container_of(art_io, struct io_spidev, art_io);
	return read(io->fd, buf, count);
}

const static struct lart_io_ops io_spidev_ops = {
	.address_type = "SPIDEV",

	.open = io_open,
	.close = io_close,

	.write = io_write,
	.read = io_read,
};

REGISTER_IO_CONTROLLER(io_spidev_ops);
