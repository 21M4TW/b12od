#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <b12od/math_utils.h>
#include <b12od/bech32_decode.h>

static uint8_t _rtable[256] = {
  //0
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_SPACE, BECH32_SPACE, BECH32_SPACE, BECH32_SPACE, BECH32_SPACE, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //32
  BECH32_SPACE, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_SPACE,  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  15,           BECH32_ERROR, 10,           17,           21,           20,           26,           30,
  7,            5,            BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //64
  BECH32_ERROR, 29,           BECH32_ERROR, 24,           13,           25,           9,            8,
  23,           BECH32_ERROR, 18,           22,           31,           27,           19,           BECH32_ERROR,
  1,            0,            3,            16,           11,           28,           12,           14,
  6,            4,            2,            BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //96
  BECH32_ERROR, 29,           BECH32_ERROR, 24,           13,           25,           9,            8,
  23,           BECH32_ERROR, 18,           22,           31,           27,           19,           BECH32_ERROR,
  1,            0,            3,            16,           11,           28,           12,           14,
  6,            4,            2,            BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //128
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //160
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //192
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,

  //224
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR,
  BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR, BECH32_ERROR
};

int bech32_decode(const char* bech32_string, char** prefix, uint8_t** data, size_t* len)
{
  size_t tot_len;
  size_t i, j=0;
  uint8_t acclen=0;
  uint16_t acc=0;
  char c=0;
  int upper=0, lower=0;
  int error;

  //Skip any initial space
  while(_rtable[(uint8_t)*bech32_string] == BECH32_SPACE) ++bech32_string;
  tot_len = strlen(bech32_string);

  //Preamble extraction. No support for white spaces within the preamble
  for(i=0; i<tot_len; ++i) {
    upper |= isupper(bech32_string[i]);
    lower |= islower(bech32_string[i]);

    if(!isalnum(bech32_string[i])) return BECH32_INVALID_CHARACTER;

    if(bech32_string[i] == '1') break;
  }

  if(i == tot_len) return BECH32_NO_PREAMBLE_DIVIDER;

  if(prefix) {
    *prefix = strndup(bech32_string, i);

    if(!*prefix) return BECH32_MEMORY_ALLOC_ISSUE;
  }
  ++i;
  *len = ceil_u64((tot_len - i)*0.625);
  *data = (uint8_t*)malloc(*len);

  if(!*data) {

    if(prefix) {
      free(*prefix);
      *prefix = NULL;
    }
    return BECH32_MEMORY_ALLOC_ISSUE;
  }

  //Decode bech32 data
  for(; i < tot_len; ++i) {
    upper |= isupper(bech32_string[i]);
    lower |= islower(bech32_string[i]);

    if(bech32_string[i] == '+') {

      for(++i; i < tot_len; ++i) {
	c = _rtable[(uint8_t)bech32_string[i]];

	if(c == BECH32_ERROR) {
	  error = BECH32_INVALID_CHARACTER;
	  goto bech32_error_cleanup;
	}

	if(c != BECH32_SPACE) break;
      }

      if(i == tot_len) break;

    } else {
      c = _rtable[(uint8_t)bech32_string[i]];

      if(c == BECH32_ERROR) {
	error = BECH32_INVALID_CHARACTER;
	goto bech32_error_cleanup;
      }
    }
    acc = (acc << 5) | c;
    acclen += 5;

    if(acclen >= 8) {
      (*data)[j++] = (acc >> (acclen-8));
      acc &= 255;
      acclen -= 8;
    }
  } 
  //Drop any residual bits

  if(upper && lower) {
      error = BECH32_UPPER_LOWER_MIX;
      goto bech32_error_cleanup;
  }

  if(j < *len) {
    *len = j;

    if(j > 0) {
      uint8_t* new_data = (uint8_t*)realloc(*data, j);

      if(new_data) *data = new_data;

    } else {
      free(*data);
      *data = NULL;
    }
  }

  return BECH32_OK;

bech32_error_cleanup:
  free(*data); *data = NULL;
  if(prefix) {
    free(*prefix);
    *prefix = NULL;
  }
  return error;
}

const char* bech32_error(const int error)
{
  switch(error) {
    case BECH32_OK:
      return "Bech32 decoding: No error";
    case BECH32_NO_PREAMBLE_DIVIDER:
      return "Bech32 decoding: Preamble divider not found";
    case BECH32_UPPER_LOWER_MIX:
      return "Bech32 decoding: Mix of lower and upper case characters";
    case BECH32_MEMORY_ALLOC_ISSUE:
      return "Bech32 decoding: Memory allocation error";
    case BECH32_INVALID_CHARACTER:
      return "Bech32 decoding: Invalid character";
    default:
      return "Bech32 decoding: Unknown error";
  }
}
