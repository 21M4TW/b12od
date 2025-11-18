#ifndef _BOLT12_TLV
#define _BOLT12_TLV

#include<stdint.h>
#include<stdlib.h>

struct tlv_record
{
  uint64_t type;
  uint64_t length;
  uint8_t* value;
};

inline static void free_tlv(struct tlv_record* tlv){free(tlv->value);}

size_t read_tlv(uint8_t const* buf, size_t maxlength, struct tlv_record* tlv);

#endif
