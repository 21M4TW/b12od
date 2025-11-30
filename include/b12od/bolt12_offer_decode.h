#ifndef _BOLT12_OFFER_DECODE_
#define _BOLT12_OFFER_DECODE_

#include <string.h>

#include <b12od/bolt12_decode.h>
#include <b12od/bolt12_types.h>

struct bolt12_offer
{
  _BOLT12_OBJECT_STRUCT_FIELDS
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

int64_t bolt12_offer_field_processor(struct bolt12_object* b12);
int64_t bolt12_offer_record_processor(struct bolt12_object* b12);

static inline void init_bolt12_offer(struct bolt12_offer* b12)
{
  memset(b12, 0, sizeof(struct bolt12_offer));
  b12->field_processor = bolt12_offer_field_processor;
  b12->record_processor = bolt12_offer_record_processor;
  b12->expected_prefix = "LNO";
}

#endif
