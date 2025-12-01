#ifndef _BOLT12_JSON_TLV_
#define _BOLT12_JSON_TLV_

#include <stdio.h>
#include <inttypes.h>

#include <b12od/bolt12_json.h>
#include <b12od/bebuf.h>

inline static int currency_string_value_func_noalloc(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;
  _tobb8_noalloc(bb, '\"');

  if((ret=_tobb_utf8_enc_noalloc(bb, data, dlen, SC_AA_MASK)) < 0) return ret;

  if(ret != 3) return -2;
  _tobb8_noalloc(bb, '\"');
  return dlen;
}

#define currency_string_value_maxlength string_value_maxlength

JSON_ADD_NAME_VALUE_DEF(currency_string);

inline static int tu64_value_func_noalloc(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;

  //Trailing 0 is fine because json_add_ ## vname ## _value allocates an extra
  //character for possible "," separator
  if((ret=sprintf((char*)bb->buf + bb->size, "%"PRIu64, betbuftoh64(data, dlen))) < 0) return ret;
  bb->size += ret;
  return dlen;
}

#define u64_value_maxlength(nbytes) (20 + 1) // includes + 1 for possible ','
#define tu64_value_maxlength u64_value_maxlength

JSON_ADD_NAME_VALUE_DEF(tu64);

#define hex_string_32B_value_func_noalloc hex_string_value_func_noalloc
#define hex_string_32B_element_length (CHAIN_HASH_LENGTH)
#define hex_string_32B_value_maxlength hex_string_value_maxlength

JSON_ADD_NAME_FIXED_ARRAY_DEF(hex_string_32B);

int blinded_path_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb);

JSON_ADD_NAME_VARIABLE_ARRAY_DEF(blinded_path);

inline static int bolt12_json_add_offer_chains(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_fixed_array_tlv(b12j, "offer_chains", hex_string_32B, tlv);}

inline static int bolt12_json_add_offer_metadata(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_metadata", hex_string, tlv);}

inline static int bolt12_json_add_offer_currency(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_currency", currency_string, tlv);}

inline static int bolt12_json_add_offer_amount(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_amount", tu64, tlv);}

inline static int bolt12_json_add_offer_description(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_description", string, tlv);}

inline static int bolt12_json_add_offer_features(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_features", hex_string, tlv);}

inline static int bolt12_json_add_offer_absolute_expiry(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_absolute_expiry", tu64, tlv);}

inline static int bolt12_json_add_offer_paths(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_variable_array_tlv(b12j, "offer_paths", blinded_path, tlv);}

inline static int bolt12_json_add_offer_issuer(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_issuer", string, tlv);}

inline static int bolt12_json_add_offer_quantity_max(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_quantity_max", tu64, tlv);}

inline static int bolt12_json_add_offer_issuer_id(struct bolt12_json* const b12j, struct tlv_record const* const tlv){return bolt12_json_add_value_tlv(b12j, "offer_issuer_id", hex_string, tlv);}

inline static int bolt12_json_add_unknown_tlv_field(struct bolt12_json* const b12j, struct tlv_record const* const tlv)
{
  int ret;
  if((ret=tobb_reserve_for(&b12j->jctx.bb, 2 + u64_value_maxlength(ERROR) + hex_string_value_maxlength(tlv->length)))) return ret; // (Both u64_value_maxlength and hex_string_value_maxlength include one extra byte)

  if((ret = sprintf((char*)b12j->jctx.bb.buf + b12j->jctx.bb.size, "\"%"PRIu64"\":", tlv->type)) < 0) return ret;
  b12j->jctx.bb.size += ret;
  hex_string_value_func_noalloc(tlv->value, tlv->length, &b12j->jctx.bb);
  return 0;
}

#endif
