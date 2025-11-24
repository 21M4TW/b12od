#ifndef _BOLT12_JSON_TLV_
#define _BOLT12_JSON_TLV_

#include <stdio.h>
#include <inttypes.h>
#include "bolt12_json.h"
#include "bebuf.h"

inline static int currency_string_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  int ret;
  _tobb8_unsafe(bb, '\"');

  if((ret=_tobb_utf8_enc_unsafe(bb, data, dlen, SC_AA_MASK)) < 0) return ret;

  if(ret != 3) return -2;
  _tobb8_unsafe(bb, '\"');
  return ret+2;
}

#define currency_string_value_maxlength(nbytes) (nbytes+2)

JSON_ADD_VALUE_DEF(currency_string);
JSON_ADD_NAME_VALUE_DEF(currency_string);

inline static int tu64_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  int ret;

  //Trailing 0 is fine because json_add_ ## vname ## _value allocates an extra
  //character for possible "," separator
  if((ret=sprintf((char*)bb->buf, "%"PRIu64, betbuftoh64(data, dlen))) < 0) return ret;
  return ret;
}

#define tu64_value_maxlength(nbytes) (20)

JSON_ADD_VALUE_DEF(tu64);
JSON_ADD_NAME_VALUE_DEF(tu64);

#define hex_string_32B_value_func hex_string_value_func
#define hex_string_32B_element_length (CHAIN_HASH_LENGTH)
#define hex_string_32B_value_maxlength hex_string_value_maxlength

JSON_ADD_NAME_FIXED_ARRAY_DEF(hex_string_32B);

inline static int bolt12_json_add_offer_chains(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_fixed_array_tlv(b12j, "offer_chains", hex_string_32B, tlv);}

inline static int bolt12_json_add_offer_metadata(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_metadata", hex_string, tlv);}

inline static int bolt12_json_add_offer_currency(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_currency", currency_string, tlv);}

inline static int bolt12_json_add_offer_amount(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_amount", tu64, tlv);}

inline static int bolt12_json_add_offer_description(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_description", string, tlv);}

inline static int bolt12_json_add_offer_features(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_features", hex_string, tlv);}

inline static int bolt12_json_add_offer_absolute_expiry(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_absolute_expiry", tu64, tlv);}

int bolt12_json_add_offer_paths(struct bolt12_json* const b12j, struct tlv_record const* const tlv);

inline static int bolt12_json_add_offer_issuer(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_issuer", string, tlv);}

inline static int bolt12_json_add_offer_quantity_max(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_quantity_max", tu64, tlv);}

inline static int bolt12_json_add_offer_issuer_id(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_issuer_id", hex_string, tlv);}

#endif
