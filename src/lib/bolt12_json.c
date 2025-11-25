#include "bolt12_offer_json.h"
#include "bolt12_json_tlv.h"

int bolt12_json(struct bolt12_json* const b12j, const char* const bolt12_string)
{
  int64_t ret;

  if((ret = bolt12_decode(bolt12_string, b12j->b12)) != BOLT12_OK) return bolt12_json_error(b12j, ret);
  return 0;
}
