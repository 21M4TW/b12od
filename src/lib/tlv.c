#include <string.h>

#include <b12od/tlv.h>
#include <b12od/bigsize.h>

size_t read_tlv(byte const* buf, size_t maxlength, struct tlv_record* tlv)
{
  byte ret, ret2;

  if(maxlength < 2) return 0;
  ret = read_bigsize(buf, maxlength, &tlv->type);
  ret2 = read_bigsize(buf + ret, maxlength - ret, &tlv->length);

  if(!ret || !ret2) return 0;
  ret += ret2;

  if(maxlength-ret < tlv->length) return 0;
  tlv->value = (byte*)malloc(tlv->length);

  if(!tlv->value) return 0;
  memcpy(tlv->value, buf+ret, tlv->length);
  return ret+tlv->length;
}
