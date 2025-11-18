#include "bolt12_offer_decode.h"

int bolt12_offer_processor(struct tlv_record const* const records, const size_t nrecords, void* data)
{
  struct tlv_record const* const last_record = records + nrecords-1;
  struct bolt12_offer* b12 = (struct bolt12_offer*)data;

  switch(last_record->type){
    case TYPE_OFFER_CHAINS:
      b12->chains = last_record;
      break;

    case TYPE_OFFER_METADATA:
      b12->metadata = last_record;
      break;

    case TYPE_OFFER_CURRENCY:
      b12->currency = last_record;
      break;

    case TYPE_OFFER_AMOUNT:
      b12->amount = last_record;
      break;

    case TYPE_OFFER_DESCRIPTION:
      b12->description = last_record;
      break;

    case TYPE_OFFER_FEATURES:
      b12->features = last_record;
      break;

    case TYPE_OFFER_ABSOLUTE_EXPIRY:
      b12->absolute_expiry = last_record;
      break;

    case TYPE_OFFER_PATHS:
      b12->paths = last_record;
      break;

    case TYPE_OFFER_ISSUER:
      b12->issuer = last_record;
      break;

    case TYPE_OFFER_QUANTITY_MAX:
      b12->quantity_max = last_record;
      break;

    case TYPE_OFFER_ISSUER_ID:
      b12->issuer_id = last_record;
      break;
    
    default:
      return 0;
  };
  return 1;
}
