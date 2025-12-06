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

inline static void htobebuf16(const uint16_t value, uint8_t* const buf){buf[0] = (value>>8); buf[1] = value;}
inline static void htobebuf32(const uint32_t value, uint8_t* const buf){buf[0] = (value>>24); buf[1] = (value>>16); buf[2] = (value>>8); buf[3] = value;}
inline static void htobebuf64(const uint64_t value, uint8_t* const buf){buf[0] = (value>>56); buf[1] = (value>>48); buf[2] = (value>>40); buf[3] = (value>>32); buf[4] = (value>>24); buf[5] = (value>>16); buf[6] = (value>>8); buf[7] = value;}

#endif
