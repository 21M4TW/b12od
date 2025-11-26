#ifndef _BOLT12_OFFER_JSON_
#define _BOLT12_OFFER_JSON_

#include "bolt12_offer_decode.h"
#include "bolt12_json.h"

int bolt12_offer_json_init(struct bolt12_json* const b12j);

#define bolt12_offer_json bolt12_json

#define bolt12_offer_json_error bolt12_json_error

inline static void bolt12_offer_json_free(struct bolt12_json* const b12j){bolt12_free_records(b12j->b12); free(b12j->b12); bolt12_json_free(b12j);}

#define bolt12_offer_json_shrink_to_fit bolt12_json_shrink_to_fit

#endif
