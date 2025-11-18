#ifndef _BOLT12_DECODE_
#define _BOLT12_DECODE_

#include "tlv.h"

enum eBolt12DecodeErrors{BOLT12_OK=0, BOLT12_INVALID_BECH32_PREFIX=8, BOLT12_INVALID_TLV_ORDERING=9, BOLT12_INVALID_TLV_RECORD=10, BOLT12_UNKNOWN_EVEN_TLV_TYPE=11};

struct bolt12_object
{
  int (*processor)(struct tlv_record const* const records, const size_t nrecords, void* data);
};
typedef void* bolt12_object_ptr;

int bolt12_decode(const char* string, bolt12_object_ptr bolt12, const char* expected_prefix, struct tlv_record** records, size_t* nrecords);

void free_bolt12_records(struct tlv_record* const records, const size_t nrecords);

#endif
