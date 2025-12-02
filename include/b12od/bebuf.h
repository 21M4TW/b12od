#ifndef _BEBUF_
#define _BEBUF_

#include <stdint.h>

static inline uint16_t bebuf16toh(uint8_t const* const buf){return ((uint16_t)buf[0])<<8|buf[1];}
static inline uint32_t bebuf32toh(uint8_t const* const buf){return ((uint32_t)bebuf16toh(buf))<<16|bebuf16toh(buf+2);}
static inline uint64_t bebuf64toh(uint8_t const* const buf){return ((uint64_t)bebuf32toh(buf))<<32|bebuf32toh(buf+4);}

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

inline static void htobebuf16(const uint16_t value, uint8_t* const buf){*(uint16_t*)buf = ((uint16_t)((uint8_t*)&value)[0])<<8|((uint8_t*)&value)[1];}
inline static void htobebuf32(const uint32_t value, uint8_t* const buf){*(uint32_t*)buf = ((uint32_t)((uint8_t*)&value)[0])<<24|((uint32_t)((uint8_t*)&value)[1])<<16|((uint16_t)((uint8_t*)&value)[2])<<8|((uint8_t*)&value)[3];}
inline static void htobebuf64(const uint64_t value, uint8_t* const buf){*(uint64_t*)buf = ((uint64_t)((uint8_t*)&value)[0])<<56|((uint64_t)((uint8_t*)&value)[1])<<48|((uint64_t)((uint8_t*)&value)[2])<<40|((uint64_t)((uint8_t*)&value)[3])<<32|((uint32_t)((uint8_t*)&value)[4])<<24|((uint32_t)((uint8_t*)&value)[5])<<16|((uint16_t)((uint8_t*)&value)[6])<<8|((uint8_t*)&value)[7];}

#endif
