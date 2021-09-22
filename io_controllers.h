#ifndef __io_controllers_h__
#define __io_controllers_h__

#include <stddef.h>

#define container_of(ptr, type, member) ({ 	\
	const typeof( ((type *)0)->member ) 	\
	*__mptr = (ptr);			\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#include "io.h"

#include <unistd.h>
#include <inttypes.h>

struct lart_io_ops {
	char *address_type;

	struct lart_io *(*open)(const char *uri);
	int (*close)(struct lart_io *art_io);

	ssize_t (*write)(struct lart_io *art_io, const void *buf, size_t count);
	ssize_t (*read)(struct lart_io *art_io, void *buf, size_t count);

	ssize_t (*write_reg8)(struct lart_io *art_io, int reg, uint8_t value);
	ssize_t (*write_reg16)(struct lart_io *art_io, int reg, uint16_t value);
	ssize_t (*write_reg32)(struct lart_io *art_io, int reg, uint32_t value);
	ssize_t (*write_reg64)(struct lart_io *art_io, int reg, uint64_t value);

	ssize_t (*read_reg8)(struct lart_io *art_io, int reg, uint8_t *value);
	ssize_t (*read_reg16)(struct lart_io *art_io, int reg, uint16_t *value);
	ssize_t (*read_reg32)(struct lart_io *art_io, int reg, uint32_t *value);
	ssize_t (*read_reg64)(struct lart_io *art_io, int reg, uint64_t *value);
};

struct lart_io {
	const struct lart_io_ops *ops;
};

#define REGISTER_IO_CONTROLLER(s) \
	const static struct lart_io_ops *__lart_io_ops_##s	\
	__attribute((__section__("io_controllers")))		\
	__attribute((__used__)) = &s

#endif /* __io_controllers_h__ */
