#include "bigsize.h"

uint8_t read_bigsize(uint8_t const* buf, const size_t maxlength, uint64_t* value)
{
  if(maxlength == 0) return 0;

  switch(*buf) {
    case 0xfd:
      if(maxlength < 3) return 0;
      *value = ((uint16_t)buf[1]<<8)|buf[2];
      return 2;
    case 0xfe:
      if(maxlength < 5) return 0;
      *value = ((uint32_t)buf[1]<<24)|((uint32_t)buf[2]<<16)|((uint16_t)buf[3]<<8)|buf[4];
      return 4;
    case 0xff:
      if(maxlength < 9) return 0;
      *value = ((uint64_t)buf[1]<<56)|((uint64_t)buf[2]<<48)|((uint64_t)buf[3]<<40)|((uint64_t)buf[4]<<32)|((uint32_t)buf[5]<<24)|((uint32_t)buf[6]<<16)|((uint16_t)buf[7]<<8)|buf[8];
      return 8;
    default:
      *value = *buf;
      return 1;
  }
}

uint8_t write_bigsize(const uint64_t value, uint8_t* buf)
{
  if(value < 0xfd) {
    buf[0] = (uint8_t)value;
    return 1;

  } else if(value < 0xffff) {
    buf[0] = 0xfd;
    buf[1] = value>>8;
    buf[2] = value&0xff;
    return 3;

  } else if(value < 0xffffffff) {
    buf[0] = 0xfe;
    buf[1] = value>>24;
    buf[2] = (value>>16)&0xff;
    buf[3] = (value>>8)&0xff;
    buf[4] = value&0xff;
    return 5;

  } else {
    buf[0] = 0xff;
    buf[1] = value>>56;
    buf[2] = (value>>48)&0xff;
    buf[3] = (value>>40)&0xff;
    buf[4] = (value>>32)&0xff;
    buf[5] = (value>>24)&0xff;
    buf[6] = (value>>16)&0xff;
    buf[7] = (value>>8)&0xff;
    buf[8] = value&0xff;
    return 9;
  }
}
