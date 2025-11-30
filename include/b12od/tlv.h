#ifndef _BOLT12_TLV
#define _BOLT12_TLV

#include<stdint.h>
#include<stdlib.h>

#include <b12od/bolt12_types.h>

struct tlv_record
{
  bigsize type;
  bigsize length;
  byte* value;
};

inline static void free_tlv(struct tlv_record* tlv){free(tlv->value);}

size_t read_tlv(byte const* buf, size_t maxlength, struct tlv_record* tlv);

#endif
