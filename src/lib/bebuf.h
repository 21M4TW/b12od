#ifndef _BEBUF_
#define _BEBUF_

#include <stdint.h>
#include <endian.h>

static inline uint16_t bebuf16toh(uint8_t const* const buf){return be16toh(*(uint16_t*)(buf));}
static inline uint32_t bebuf32toh(uint8_t const* const buf){return be32toh(*(uint32_t*)(buf));}
static inline uint64_t bebuf64toh(uint8_t const* const buf){return be64toh(*(uint64_t*)(buf));}

static inline uint64_t betbuftoh64(uint8_t const* const buf, const uint8_t nbytes)
{
  switch(nbytes) {
    case 0:
      return 0;

    case 1:
      return buf[0];

    case 2:
      return bebuf16toh(buf);

    case 3:
      return ((uint32_t)bebuf16toh(buf)<<8)|buf[2];

    case 4:
      return bebuf32toh(buf);

    case 5:
      return ((uint64_t)bebuf32toh(buf)<<8)|buf[4];

    case 6:
      return ((uint64_t)bebuf32toh(buf)<<16)|bebuf16toh(buf+4);

    case 7:
      return ((uint64_t)bebuf32toh(buf)<<24)|((uint32_t)bebuf16toh(buf+4)<<8)|buf[6];

    default:
      return bebuf64toh(buf);
  }
}

static inline void htobebuf16(const uint16_t value, uint8_t* const buf){*(uint16_t*)(buf) = htobe16(value);}
static inline void htobebuf32(const uint32_t value, uint8_t* const buf){*(uint32_t*)(buf) = htobe32(value);}
static inline void htobebuf64(const uint64_t value, uint8_t* const buf){*(uint64_t*)(buf) = htobe64(value);}

#endif
