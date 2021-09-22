#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "io_controllers.h"

static const struct lart_io_ops null_controller = { 0 };
REGISTER_IO_CONTROLLER(null_controller);

struct lart_io *lart_io_open(const char *uri)
{
	if (!uri)
		return NULL;

	extern struct lart_io_ops *__start_io_controllers;
	extern struct lart_io_ops *__stop_io_controllers;
	struct lart_io_ops **ops;

	/* Call 'address_type:' associated controller */
	const size_t address_type_length = strcspn(uri, ":");
	for (ops = &__start_io_controllers; ops < &__stop_io_controllers; ops++) {
		if ((*ops)->address_type)
			if (!strncmp(uri, (*ops)->address_type, address_type_length))
				if ((*ops)->open)
					return (*ops)->open(uri);
	}

	errno = EINVAL;
	return NULL;
}

int lart_io_close(struct lart_io *io)
{
	if (io && io->ops && io->ops->close)
		return io->ops->close(io);
	return -1;
}

ssize_t lart_io_write(struct lart_io *io, const void *buf, size_t count)
{
	if (io && io->ops && io->ops->write)
		return io->ops->write(io, buf, count);
	return -1;
}

ssize_t lart_io_read(struct lart_io *io, void *buf, size_t count)
{
	if (io && io->ops && io->ops->read)
		return io->ops->read(io, buf, count);
	return -1;
}

ssize_t lart_io_write_reg8(struct lart_io *io, int reg, uint8_t value)
{
	if (io && io->ops && io->ops->write_reg8)
		return io->ops->write_reg8(io, reg, value);
	return -1;
}

ssize_t lart_io_write_reg16(struct lart_io *io, int reg, uint16_t value)
{
	if (io && io->ops && io->ops->write_reg16)
		return io->ops->write_reg16(io, reg, value);
	return -1;
}

ssize_t lart_io_write_reg32(struct lart_io *io, int reg, uint32_t value)
{
	if (io && io->ops && io->ops->write_reg32)
		return io->ops->write_reg32(io, reg, value);
	return -1;
}

ssize_t lart_io_write_reg64(struct lart_io *io, int reg, uint64_t value)
{
	if (io && io->ops && io->ops->write_reg64)
		return io->ops->write_reg64(io, reg, value);
	return -1;
}


ssize_t lart_io_read_reg8(struct lart_io *io, int reg, uint8_t *value)
{
	if (io && io->ops && io->ops->read_reg8)
		return io->ops->read_reg8(io, reg, value);
	return -1;
}

ssize_t lart_io_read_reg16(struct lart_io *io, int reg, uint16_t *value)
{
	if (io && io->ops && io->ops->read_reg16)
		return io->ops->read_reg16(io, reg, value);
	return -1;
}

ssize_t lart_io_read_reg32(struct lart_io *io, int reg, uint32_t *value)
{
	if (io && io->ops && io->ops->read_reg32)
		return io->ops->read_reg32(io, reg, value);
	return -1;
}

ssize_t lart_io_read_reg64(struct lart_io *io, int reg, uint64_t *value)
{
	if (io && io->ops && io->ops->read_reg64)
		return io->ops->read_reg64(io, reg, value);
	return -1;
}
