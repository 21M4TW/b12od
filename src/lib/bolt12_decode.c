#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "bolt12_decode.h"
#include "tlv.h"
#include "bech32_decode.h"

#define RSCALING (1.25)

u64 bolt12_decode(const char* string, bolt12_object_ptr bolt12)
{
  char* prefix;
  uint8_t *data, *off_data;
  size_t len;
  size_t narecords=2;
  size_t exp_pref_len, pref_len;
  size_t i;
  u64 error;
  struct bolt12_object* b12=(struct bolt12_object*)bolt12;
  b12->nrecords = 0;

  if((error = bech32_decode(string, &prefix, &data, &len)) != BECH32_OK) return (BOLT12_DECODE_BECH32_ERROR_BASE+error);

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
	free_bolt12_records(b12);
	goto bolt12_error_cleanup;
      }
      --b12->nrecords;
    }

    if(b12->nrecords == narecords) {
      narecords = ceil(narecords*RSCALING);
      b12->records = (struct tlv_record*)realloc(b12->records, narecords*sizeof(struct tlv_record));
    }
  }

  if(len > 0) {
    free_bolt12_records(b12);
    error = BOLT12_INVALID_TLV_RECORD;
    goto bolt12_error_cleanup;
  }

  error = b12->record_processor(b12);

  if(error != BOLT12_OK) {
    free_bolt12_records(b12);
    goto bolt12_error_cleanup;
  }
  b12->records = (struct tlv_record*)realloc(b12->records, b12->nrecords*sizeof(struct tlv_record));
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
