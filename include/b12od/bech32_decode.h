#ifndef _BOLT12_BECH32_
#define _BOLT12_BECH32_

#include<stddef.h>
#include<stdint.h>
#include<inttypes.h>

enum eBech32DecodeErrors{BECH32_OK=0, BECH32_MEMORY_ALLOC_ISSUE=INT32_MIN, BECH32_NO_PREAMBLE_DIVIDER=1, BECH32_UPPER_LOWER_MIX=2, BECH32_INVALID_CHARACTER=3};

enum eBech32SpecialTypes{BECH32_SPACE=126, BECH32_ERROR=127};

int bech32_decode(const char* bech32_string, char** prefix, uint8_t** data, size_t* len);

const char* bech32_error(const int error);

#endif
