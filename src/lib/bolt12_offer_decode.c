#include <b12od/bolt12_offer_decode.h>
#include <b12od/bolt12_tlv_field_check.h>

int64_t bolt12_offer_field_processor(struct bolt12_object* b12)
{
  struct tlv_record const* const last_record = b12->records + b12->nrecords-1;
  struct bolt12_offer* b12_offer = (struct bolt12_offer*)b12;

  if(last_record->type < 1 || last_record->type > 1999999999 || (last_record->type > 79 && last_record->type < 1000000000)) return BOLT12_INVALID_TLV_TYPE;

  switch(last_record->type){
    case TYPE_OFFER_CHAINS:
      if(!check_offer_chains(last_record)) return last_record->type;
      b12_offer->chains = last_record;
      break;

    case TYPE_OFFER_METADATA:
      b12_offer->metadata = last_record;
      break;

    case TYPE_OFFER_CURRENCY:
      if(!check_offer_currency(last_record)) return last_record->type;
      b12_offer->currency = last_record;
      break;

    case TYPE_OFFER_AMOUNT:
      if(!check_offer_amount(last_record)) return last_record->type;
      b12_offer->amount = last_record;
      break;

    case TYPE_OFFER_DESCRIPTION:
      b12_offer->description = last_record;
      break;

    case TYPE_OFFER_FEATURES:
      b12_offer->features = last_record;
      break;

    case TYPE_OFFER_ABSOLUTE_EXPIRY:
      if(!check_offer_absolute_expiry(last_record)) return last_record->type;
      b12_offer->absolute_expiry = last_record;
      break;

    case TYPE_OFFER_PATHS:
      if(!check_offer_paths(last_record)) return last_record->type;
      b12_offer->paths = last_record;
      break;

    case TYPE_OFFER_ISSUER:
      b12_offer->issuer = last_record;
      break;

    case TYPE_OFFER_QUANTITY_MAX:
      if(!check_offer_quantity_max(last_record)) return last_record->type;
      b12_offer->quantity_max = last_record;
      break;

    case TYPE_OFFER_ISSUER_ID:
      if(!check_offer_issuer_id(last_record)) return last_record->type;
      b12_offer->issuer_id = last_record;
      break;
    
    default:
      return BOLT12_UNKNOWN_TLV_TYPE;
  };
  return BOLT12_OK;
}

int64_t bolt12_offer_record_processor(struct bolt12_object* b12)
{
  struct bolt12_offer const* const b12_offer = (struct bolt12_offer const*)b12;

  if(b12_offer->amount && !b12_offer->description) return TYPE_OFFER_DESCRIPTION;

  if(b12_offer->currency && !b12_offer->amount) return TYPE_OFFER_AMOUNT;

  if(!b12_offer->paths && !b12_offer->issuer_id) return TYPE_OFFER_PATHS;
  return BOLT12_OK;
}
