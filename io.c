#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "io_controllers.h"

static const struct lart_io_ops null_controller = { 0 };
REGISTER_IO_CONTROLLER(null_controller);

int lart_io_parse_uri(const char *uri, struct lart_io_uri *io_uri)
{
	if (!io_uri)
		return -1;

	const size_t address_type_length = strcspn(uri, ":");
	if (address_type_length >= sizeof(io_uri->address_type))
		return -1;

	io_uri->address_type = strndup(uri, address_type_length);

	int i;
	const char *next_slot = uri + address_type_length;
	for (i = 0; i < LART_IO_URI_MAX_PARAMETERS; i++) {
		io_uri->parameters[i] = NULL;

		if (*next_slot == '\0')
			return 0;
		if (*next_slot == ',')
			break;
		next_slot++;
		const size_t slot_length = strcspn(next_slot, ":,");
		if (!slot_length)
			return 0;

		io_uri->parameters[i] = strndup(next_slot, slot_length);
		next_slot += slot_length;
	}

	for (i = 0; i < LART_IO_URI_MAX_OPTIONS; i++) {
		io_uri->options[i] = NULL;

		if (*next_slot == '\0')
			return 0;

		next_slot++;
		const size_t slot_length = strcspn(next_slot, ",");
		if (!slot_length)
			return 0;

		io_uri->options[i] = strndup(next_slot, slot_length);
		next_slot += slot_length;
	}

	return 0;
}

void lart_free_uri(struct lart_io_uri *io_uri)
{
	if (!io_uri)
		return;

	free(io_uri->address_type);
	int i;
	for (i = 0; i < LART_IO_URI_MAX_PARAMETERS && io_uri->parameters[i]; i++)
		free(io_uri->parameters[i]);

	for (i = 0; i < LART_IO_URI_MAX_OPTIONS && io_uri->options[i]; i++)
		free(io_uri->options[i]);
}

struct lart_io *lart_io_open(const char *uri)
{
	errno = EINVAL;

	struct lart_io_uri io_uri;
	if (lart_io_parse_uri(uri, &io_uri) < 0)
		return NULL;

	extern struct lart_io_ops *__start_io_controllers;
	extern struct lart_io_ops *__stop_io_controllers;
	struct lart_io_ops **ops;

	struct lart_io *output = NULL;

	/* Call 'address_type:' associated controller */
	for (ops = &__start_io_controllers; ops < &__stop_io_controllers; ops++) {
		if ((*ops)->address_type) {
			if (!strcmp(io_uri.address_type, (*ops)->address_type)) {
				if ((*ops)->open) {
					output = (*ops)->open(&io_uri);
					goto out;
				}
			}
		}
	}

out:
	lart_free_uri(&io_uri);
	return output;
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
