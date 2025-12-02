#ifndef _BOLT12_OFFER_JSON_
#define _BOLT12_OFFER_JSON_

#include <b12od/bolt12_offer_decode.h>
#include <b12od/bolt12_json.h>

struct bolt12_json* bolt12_offer_json_new();

#define bolt12_offer_json bolt12_json

#define bolt12_offer_json_error bolt12_json_error

void bolt12_offer_json_delete(struct bolt12_json* const b12j);

void bolt12_offer_json_shrink_to_fit(struct bolt12_json* const b12j);

size_t bolt12_offer_json_get_size(struct bolt12_json* const b12j);

#endif
