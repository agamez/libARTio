#ifndef __io_h__
#define __io_h__

#include <unistd.h>
#include <inttypes.h>

struct lart_io;

struct lart_io *lart_io_open(const char *uri);
int lart_io_close(struct lart_io *art_io);

ssize_t lart_io_write(struct lart_io *art_io, const void *buf, size_t count);
ssize_t lart_io_read(struct lart_io *art_io, void *buf, size_t count);

ssize_t lart_io_write_reg8(struct lart_io *art_io, int reg, uint8_t value);
ssize_t lart_io_write_reg16(struct lart_io *art_io, int reg, uint16_t value);
ssize_t lart_io_write_reg32(struct lart_io *art_io, int reg, uint32_t value);
ssize_t lart_io_write_reg64(struct lart_io *art_io, int reg, uint64_t value);

ssize_t lart_io_read_reg8(struct lart_io *art_io, int reg, uint8_t *value);
ssize_t lart_io_read_reg16(struct lart_io *art_io, int reg, uint16_t *value);
ssize_t lart_io_read_reg32(struct lart_io *art_io, int reg, uint32_t *value);
ssize_t lart_io_read_reg64(struct lart_io *art_io, int reg, uint64_t *value);

#endif /* __io_h__ */
