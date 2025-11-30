#ifndef _HEX_ENC_
#define _HEX_ENC_

#include<stdint.h>

extern const uint8_t _htable[16];

inline static void hex8(const uint8_t value, char buf[2]){buf[0] = _htable[value>>4]; buf[1] = _htable[value&0xf];}

inline static void hex16(const uint16_t value, char buf[4]){hex8((uint8_t)(value>>8), buf); hex8((uint8_t)(value), buf+2);}

inline static void hex32(const uint32_t value, char buf[8]){hex16((uint16_t)(value>>16), buf); hex16((uint16_t)(value), buf+4);}

inline static void hex64(const uint64_t value, char buf[16]){hex32((uint32_t)(value>>32), buf); hex32((uint32_t)(value), buf+8);}

#endif
