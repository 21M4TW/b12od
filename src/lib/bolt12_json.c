#include <b12od/bolt12_offer_json.h>
#include <b12od/bolt12_json_tlv.h>

const char* bolt12_json(struct bolt12_json* const b12j, const char* const bolt12_string)
{
  int64_t ret;
  size_t type_index = 0;
  struct bolt12_object const* const b12 = (struct bolt12_object const*)b12j->b12;
  size_t i;

  bolt12_json_reset(b12j);

  if((ret = bolt12_decode(bolt12_string, b12j->b12)) != BOLT12_OK) goto json_error;

  if((ret = tobb8(&b12j->jctx.bb, '{')) < 0) goto json_error;

  //A Bolt12 object must contain at least one field
  type_index = bolt12_json_find_type_enc_func(b12j->vector, b12j->vector_length, b12->records[0].type);

  if((ret = b12j->vector[type_index].enc_func(b12j, b12->records)) < 0) goto json_error;

  for(i = 1; i < b12->nrecords; ++i) {
    _tobb8_noalloc(&b12j->jctx.bb, ',');
    type_index += bolt12_json_find_type_enc_func(b12j->vector + type_index, b12j->vector_length - type_index, b12->records[i].type);

    if((ret = b12j->vector[type_index].enc_func(b12j, b12->records + i)) < 0) goto json_error;
  }
  _tobb8_noalloc(&b12j->jctx.bb, '}');
  if((ret = tobb8(&b12j->jctx.bb, 0)) < 0) goto json_error;

  return (const char*)b12j->jctx.bb.buf;

json_error:
  ret = bolt12_json_error(b12j, ret);
  return (ret ? NULL: (const char*)b12j->jctx.bb.buf);
}

size_t bolt12_json_find_type_enc_func(struct bolt12_json_vector_element const* const vector, const size_t nelements, const bigsize type)
{
  if(vector[0].type == type) return 0;

  if(vector[0].type > type || vector[nelements - 1].type < type) return nelements;

  size_t index = 1;
  ssize_t delta = nelements - 2;
  size_t half_delta = delta>>1;
  size_t middle;

  while(delta > 0) {
    middle = index + half_delta;

    if(type > vector[middle].type) index = ++middle;
    delta = half_delta;
    half_delta = delta>>1;
  }

  if(vector[index].type == type) return index;
  return nelements;
}
