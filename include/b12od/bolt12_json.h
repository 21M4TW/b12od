#ifndef _BOLT12_JSON_
#define _BOLT12_JSON_

#include <b12od/bolt12_decode.h>
#include <b12od/json_encode.h>

struct bolt12_json;

// Important: Vector elements must be sorted
struct bolt12_json_vector_element
{
  bigsize type;
  int (*enc_func)(struct bolt12_json* const b12j, struct tlv_record const* const tlv);
};

struct bolt12_json
{
  bolt12_object_ptr b12;
  struct json jctx;
  struct bolt12_json_vector_element const* vector;
  size_t vector_length;
};

inline static struct bolt12_json* bolt12_json_new()
{
  struct bolt12_json* const b12j = (struct bolt12_json*)malloc(sizeof(struct bolt12_json));

  if(!b12j) return NULL;

  if(json_init(&b12j->jctx)) {
    free(b12j);
    return NULL;
  }
  return b12j;
}

#define bolt12_json_add_value_tlv(b12j, name, vname, tlv) json_add_name_value(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

#define bolt12_json_add_fixed_array_tlv(b12j, name, vname, tlv) json_add_name_fixed_array(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

#define bolt12_json_add_variable_array_tlv(b12j, name, vname, tlv) json_add_name_variable_array(&(b12j)->jctx, name, vname, (tlv)->value, (tlv)->length)

const char* bolt12_json(struct bolt12_json* const b12j, const char* const bolt12_string);

inline static int bolt12_json_error(struct bolt12_json* const b12j, const int64_t error_code){return json_error(&b12j->jctx, error_code, bolt12_error(error_code));}

inline static void bolt12_json_delete(struct bolt12_json* const b12j){json_free(&b12j->jctx); free(b12j);}

inline static void bolt12_json_reset(struct bolt12_json* const b12j){json_reset(&b12j->jctx);}

inline static void bolt12_json_shrink_to_fit(struct bolt12_json* const b12j){json_shrink_to_fit(&b12j->jctx);}

size_t bolt12_json_find_type_enc_func(struct bolt12_json_vector_element const* const vector, const size_t nelements, const bigsize type);

#endif
