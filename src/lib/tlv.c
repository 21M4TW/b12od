#include <string.h>
#include "tlv.h"
#include "bigsize.h"

size_t read_tlv(byte const* buf, size_t maxlength, struct tlv_record* tlv)
{
  if(maxlength < 2) return 0;
  byte ret = read_bigsize(buf, maxlength, &tlv->type);

  if(!ret) return 0;
  ret += read_bigsize(buf + ret, maxlength - ret, &tlv->length);

  if(maxlength-ret < tlv->length) return 0;
  tlv->value = (byte*)malloc(tlv->length);

  if(!tlv->value) return 0;
  memcpy(tlv->value, buf+ret, tlv->length);
  return ret+tlv->length;
}
