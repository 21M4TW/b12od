#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "bolt12_decode.h"
#include "tlv.h"
#include "bech32_decode.h"

#define RSCALING (1.25)

int bolt12_decode(const char* string, bolt12_object_ptr bolt12, const char* expected_prefix, struct tlv_record** records, size_t* nrecords)
{
  char* prefix;
  uint8_t *data, *off_data;
  size_t len;
  size_t narecords=2;
  *nrecords=0;
  size_t exp_pref_len, pref_len;
  size_t i;
  int error;
  struct bolt12_object* b12=(struct bolt12_object*)bolt12;

  if((error = bech32_decode(string, &prefix, &data, &len)) != BECH32_OK) return error;

  off_data = data;
  exp_pref_len = strlen(expected_prefix);
  pref_len = strlen(prefix);

  if(pref_len != exp_pref_len) {
    error = BOLT12_INVALID_BECH32_PREFIX;
    goto bolt12_error_cleanup;
  }

  for(i=0; i < pref_len; ++i)

    if(tolower(prefix[i]) != tolower(expected_prefix[i])) {
      error = BOLT12_INVALID_BECH32_PREFIX;
      goto bolt12_error_cleanup;
    }

  *records = (struct tlv_record*)malloc(narecords*sizeof(struct tlv_record));

  while(len > 0) {
    i = read_tlv(off_data, len, (*records)+*nrecords);

    if(i==0) break;
    //printf("Field type %lu with length %lu\n", (*records)[*nrecords].type, (*records)[*nrecords].length);
    len -= i;
    off_data += i;
    ++*nrecords;

    if(*nrecords > 1 && (*records)[*nrecords-1].type <= (*records)[*nrecords-2].type) {
      free_bolt12_records(*records, *nrecords);
      error = BOLT12_INVALID_TLV_ORDERING;
      goto bolt12_error_cleanup;
    }

    if(b12->processor(*records, *nrecords, b12) == 0) {

      //Even unknown types cause a failure
      if(((*records)[*nrecords-1].type%2) == 0) {
	free_bolt12_records(*records, *nrecords);
	error = BOLT12_UNKNOWN_EVEN_TLV_TYPE;
	goto bolt12_error_cleanup;
      }
      --*nrecords;
    }

    if(*nrecords == narecords) {
      narecords = ceil(narecords*RSCALING);
      *records = (struct tlv_record*)realloc(*records, narecords*sizeof(struct tlv_record));
    }
  }

  if(len > 0) {
    free_bolt12_records(*records, *nrecords);
    error = BOLT12_INVALID_TLV_RECORD;
    goto bolt12_error_cleanup;
  }
  *records = (struct tlv_record*)realloc(*records, *nrecords*sizeof(struct tlv_record));
  return BOLT12_OK;

bolt12_error_cleanup:
  free(prefix);
  free(data);
  *nrecords = 0;
  return error;
}

void free_bolt12_records(struct tlv_record* const records, const size_t nrecords)
{
  int i;

  for(i=0; i < nrecords; ++i) free_tlv(records+i);
  free(records);
}
