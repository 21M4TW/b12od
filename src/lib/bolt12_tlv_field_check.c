#include "bolt12_types.h"
#include "bebuf.h"
#include "bolt12_tlv_field_check.h"

int check_offer_paths(struct tlv_record const* const record)
{
  if(record->length < 2*EC_POINT_LENGTH + SCIDDIR_LENGTH + 3) return 0;
  byte nhops;
  byte h;
  u16 dlen;
  u16 d;
  u64 pos = (record->value[0] < 2 ? SCIDDIR_LENGTH + EC_POINT_LENGTH : 2*EC_POINT_LENGTH);

  while(pos + 1 < record->length) {
    nhops = record->value[pos];

    if(nhops == 0) return 0; 
    ++pos;

    for(h=0; h<nhops; ++h) {
      pos += EC_POINT_LENGTH;

      if(record->length < pos + 2) return 0;
      dlen = bebuf16toh(record->value+pos);
      pos += 2 + dlen;

      if(record->length < pos) return 0;
    }

    if(pos == record->length) return 1;
    pos += 2*EC_POINT_LENGTH;
  }
  return 0;
}
