#ifndef _BOLT12_OFFER_JSON_
#define _BOLT12_OFFER_JSON_

#include "bolt12_json.h"

inline static int bolt12_offer_json_init(struct bolt12_json* const b12j)
{
  b12j->b12 = malloc(sizeof(struct bolt12_offer));

  if(!b12j->b12) return -1;
  init_bolt12_offer(b12j->b12);
  return json_init(&b12j->jctx);
}

#define bolt12_offer_json_error bolt12_json_error

inline static void bolt12_offer_json_free(struct bolt12_json* const b12j){bolt12_free_record(b12j->b12); free(b12j->b12); json_free(&b12j->jctx);}

#endif
