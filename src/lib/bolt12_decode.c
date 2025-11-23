#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "bolt12_decode.h"
#include "tlv.h"
#include "bech32_decode.h"

#define RSCALING (1.25)

int64_t bolt12_decode(const char* string, bolt12_object_ptr bolt12)
{
  char* prefix;
  uint8_t *data, *off_data;
  size_t len;
  size_t narecords=2;
  size_t exp_pref_len, pref_len;
  size_t i;
  int64_t error;
  struct bolt12_object* b12=(struct bolt12_object*)bolt12;
  b12->nrecords = 0;

  if((error = bech32_decode(string, &prefix, &data, &len)) != BECH32_OK) return (error>0? BOLT12_DECODE_BECH32_ERROR_BASE+error: error);

  off_data = data;
  exp_pref_len = strlen(b12->expected_prefix);
  pref_len = strlen(prefix);

  if(pref_len != exp_pref_len) {
    error = BOLT12_INVALID_BECH32_PREFIX;
    goto bolt12_error_cleanup;
  }

  for(i=0; i < pref_len; ++i)

    if(tolower(prefix[i]) != tolower(b12->expected_prefix[i])) {
      error = BOLT12_INVALID_BECH32_PREFIX;
      goto bolt12_error_cleanup;
    }

  b12->records = (struct tlv_record*)malloc(narecords*sizeof(struct tlv_record));

  if(!b12->records) {
    error = BOLT12_MEMORY_ALLOC_ISSUE;
    goto bolt12_error_cleanup;
  }

  while(len > 0) {
    i = read_tlv(off_data, len, (b12->records)+b12->nrecords);

    if(i==0) break;
    //printf("Field type %lu with length %lu\n", (b12->records)[b12->nrecords].type, (b12->records)[b12->nrecords].length);
    len -= i;
    off_data += i;
    ++b12->nrecords;

    if(b12->nrecords > 1 && (b12->records)[b12->nrecords-1].type <= (b12->records)[b12->nrecords-2].type) {
      free_bolt12_records(b12);
      error = BOLT12_INVALID_TLV_ORDERING;
      goto bolt12_error_cleanup;
    }

    error = b12->field_processor(b12);

    if(error != BOLT12_OK) {

      //Even unknown types cause a failure
      if(error != BOLT12_UNKNOWN_TLV_TYPE || ((b12->records)[b12->nrecords-1].type%2) == 0) {

	if(error > 0) error += BOLT12_DECODE_OBJECT_ERROR_BASE;
	free_bolt12_records(b12);
	goto bolt12_error_cleanup;
      }
      --b12->nrecords;
    }

    if(b12->nrecords == narecords) {
      narecords = ceil(narecords*RSCALING);
      b12->records = (struct tlv_record*)realloc(b12->records, narecords*sizeof(struct tlv_record));

      if(!b12->records) {
        free_bolt12_records(b12);
	error = BOLT12_MEMORY_ALLOC_ISSUE;
	goto bolt12_error_cleanup;
      }
    }
  }

  if(len > 0) {
    free_bolt12_records(b12);
    error = BOLT12_INVALID_TLV_RECORD;
    goto bolt12_error_cleanup;
  }

  error = b12->record_processor(b12);

  if(error != BOLT12_OK) {

    if(error > 0) error += BOLT12_DECODE_OBJECT_ERROR_BASE;
    free_bolt12_records(b12);
    goto bolt12_error_cleanup;
  }

  if(b12->nrecords < narecords) b12->records = (struct tlv_record*)realloc(b12->records, b12->nrecords*sizeof(struct tlv_record));
  return BOLT12_OK;

bolt12_error_cleanup:
  free(prefix);
  free(data);
  b12->nrecords = 0;
  return error;
}

void free_bolt12_records(struct bolt12_object* const b12)
{
  int i;

  for(i=0; i < b12->nrecords; ++i) free_tlv(b12->records+i);
  free(b12->records);
}

const char* bolt12_error(const int64_t error)
{
  if(error < 0) {

    if(error == -1) return "Bolt12 decoding: Memory allocation error";
    return "Bolt12 decoding: Unknown negative error";
  }

  if(error&BOLT12_DECODE_BECH32_ERROR_BASE) return bech32_error((int32_t)error);

  else if(error&BOLT12_DECODE_GENERAL_ERROR_BASE) {

    switch(error) {
      case BOLT12_INVALID_BECH32_PREFIX:
	return "Bolt12 decoding: Unexpected Bech32 prefix";
      case BOLT12_INVALID_TLV_ORDERING:
	return "Bolt12 decoding: The ordering of TLV fields is not strictly increasing";
      case BOLT12_INVALID_TLV_RECORD:
	return "Bolt12 decoding: Invalid TLV record";
      case BOLT12_INVALID_TLV_TYPE:
	return "Bolt12 decoding: TLV type is not within the allowed range";
      case BOLT12_UNKNOWN_TLV_TYPE:
	return "Bolt12 decoding: Unknown TLV type with even value";
    }
  }

  //TLV fields errors
  switch(error&0xFFFFFFFF) {
    case TYPE_OFFER_CHAINS:
      return "Bolt12 decoding: Invalid offer chains field";
    case TYPE_OFFER_METADATA:
      return "Bolt12 decoding: Invalid offer metadata field";
    case TYPE_OFFER_CURRENCY:
      return "Bolt12 decoding: Invalid offer currency field";
    case TYPE_OFFER_AMOUNT:
      return "Bolt12 decoding: Invalid offer amount field";
    case TYPE_OFFER_DESCRIPTION:
      return "Bolt12 decoding: Invalid offer description field";
    case TYPE_OFFER_FEATURES:
      return "Bolt12 decoding: Invalid offer features field";
    case TYPE_OFFER_ABSOLUTE_EXPIRY:
      return "Bolt12 decoding: Invalid offer absolute expiry field";
    case TYPE_OFFER_PATHS:
      return "Bolt12 decoding: Invalid offer paths field";
    case TYPE_OFFER_ISSUER:
      return "Bolt12 decoding: Invalid offer issuer field";
    case TYPE_OFFER_QUANTITY_MAX:
      return "Bolt12 decoding: Invalid offer quantity field";
    case TYPE_OFFER_ISSUER_ID:
      return "Bolt12 decoding: Invalid offer issuer ID field";
    default:
      return "Bolt12 decoding: Unknown error";
  }
}
