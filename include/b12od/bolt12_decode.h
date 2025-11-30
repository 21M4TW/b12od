#ifndef _BOLT12_DECODE_
#define _BOLT12_DECODE_

#include <inttypes.h>

#include <b12od/tlv.h>

#define BOLT12_DECODE_GENERAL_ERROR_BASE (0x200000000000000)
#define BOLT12_DECODE_BECH32_ERROR_BASE  (0x400000000000000)

enum eBolt12DecodeErrors{
  BOLT12_OK = 0,
  BOLT12_MEMORY_ALLOC_ISSUE = INT32_MIN,
  BOLT12_INVALID_BECH32_PREFIX = BOLT12_DECODE_GENERAL_ERROR_BASE+1,
  BOLT12_INVALID_TLV_ORDERING = BOLT12_DECODE_GENERAL_ERROR_BASE+2,
  BOLT12_INVALID_TLV_RECORD = BOLT12_DECODE_GENERAL_ERROR_BASE+3,
  BOLT12_INVALID_TLV_TYPE = BOLT12_DECODE_GENERAL_ERROR_BASE+4,
  BOLT12_UNKNOWN_TLV_TYPE = BOLT12_DECODE_GENERAL_ERROR_BASE+5
};

typedef void* bolt12_object_ptr;
struct bolt12_object
{
#define _BOLT12_OBJECT_STRUCT_FIELDS \
  int64_t (*field_processor)(struct bolt12_object* b12); \
  int64_t (*record_processor)(struct bolt12_object* b12); \
  const char* expected_prefix; \
  struct tlv_record* records; \
  size_t nrecords;
  _BOLT12_OBJECT_STRUCT_FIELDS
};

int64_t bolt12_decode(const char* string, bolt12_object_ptr bolt12);

void bolt12_free_records(struct bolt12_object* const b12);

const char* bolt12_error(const int64_t error);

#endif
