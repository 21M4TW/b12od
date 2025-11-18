#ifndef _BOLT12_OFFER_DECODE_
#define _BOLT12_OFFER_DECODE_

#include <string.h>
#include "bolt12_decode.h"
#include "bolt12_types.h"

struct bolt12_offer
{
  int (*processor)(struct tlv_record const* const records, const size_t nrecords, void* data);
  struct tlv_record const* chains;
  struct tlv_record const* metadata;
  struct tlv_record const* currency;
  struct tlv_record const* amount;
  struct tlv_record const* description;
  struct tlv_record const* features;
  struct tlv_record const* absolute_expiry;
  struct tlv_record const* paths;
  struct tlv_record const* issuer;
  struct tlv_record const* quantity_max;
  struct tlv_record const* issuer_id;
};

int bolt12_offer_processor(struct tlv_record const* const records, const size_t nrecords, void* data);

static inline void init_bolt12_offer(struct bolt12_offer* b12){memset(b12, 0, sizeof(struct bolt12_offer)); b12->processor = bolt12_offer_processor;}

#endif
