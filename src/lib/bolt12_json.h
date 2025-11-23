#ifndef _BOLT12_JSON_
#define _BOLT12_JSON_

#include "bolt12_decode.h"
#include "json_encode.h"

struct bolt12_json
{
  bolt12_object_ptr b12;
  struct json jctx;
};

inline static int bolt12_json_init(struct bolt12_json* const b12j)
{
  return json_init(&b12j->jctx);
}

inline static int bolt12_json_error(struct bolt12_json* const b12j, const int64_t error_code){return json_error(&b12j->jctx, bolt12_error(error_code));}

inline static void bolt12_json_free(struct bolt12_json* const b12j){json_free(&b12j->jctx);}

#endif
