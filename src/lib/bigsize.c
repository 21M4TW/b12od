#include <b12od/bebuf.h>
#include <b12od/bigsize.h>

uint8_t read_bigsize(uint8_t const* const buf, const size_t maxlength, uint64_t* const value)
{
  if(maxlength == 0) return 0;

  switch(*buf) {
    case 0xfd:
      if(maxlength < 3) return 0;
      *value = bebuf16toh(buf+1);

      if(*value < 0xfd) return 0;
      return 3;
    case 0xfe:
      if(maxlength < 5) return 0;
      *value = bebuf32toh(buf+1);

      if(*value < 0xffff) return 0;
      return 5;
    case 0xff:
      if(maxlength < 9) return 0;
      *value = bebuf64toh(buf+1);

      if(*value < 0xffffffff) return 0;
      return 9;
    default:
      *value = *buf;
      return 1;
  }
}

uint8_t write_bigsize(const uint64_t value, uint8_t* const buf)
{
  if(value < 0xfd) {
    buf[0] = (uint8_t)value;
    return 1;

  } else if(value < 0xffff) {
    buf[0] = 0xfd;
    htobebuf16(value, buf+1);
    return 3;

  } else if(value < 0xffffffff) {
    buf[0] = 0xfe;
    htobebuf32(value, buf+1);
    return 5;

  } else {
    buf[0] = 0xff;
    htobebuf64(value, buf+1);
    return 9;
  }
}
