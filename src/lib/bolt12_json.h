#ifndef _BOLT12_JSON_
#define _BOLT12_JSON_

#include "bolt12_decode.h"
#include "json_encode.h"

struct bolt12_json_vector
{
  bigsize type;
  int (*enc_func)(struct json* jctx, struct tlv_record const* const tlv);
};

struct bolt12_json
{
  bolt12_object_ptr b12;
  struct json jctx;
};

inline static int bolt12_json_init(struct bolt12_json* const b12j)
{
  return json_init(&b12j->jctx);
}

#define bolt12_json_add_value_tlv(b12j, name, vname, tlv) json_add_name_value(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

#define bolt12_json_add_fixed_array_tlv(b12j, name, vname, tlv) json_add_name_fixed_array(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

#define bolt12_json_add_variable_array_tlv(b12j, name, vname, tlv) json_add_name_variable_array(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

inline static int bolt12_json_error(struct bolt12_json* const b12j, const int64_t error_code){return json_error(&b12j->jctx, error_code, bolt12_error(error_code));}

inline static void bolt12_json_free(struct bolt12_json* const b12j){json_free(&b12j->jctx);}

#endif
