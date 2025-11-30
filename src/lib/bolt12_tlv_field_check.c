#include <b12od/bolt12_types.h>
#include <b12od/bebuf.h>
#include <b12od/bolt12_tlv_field_check.h>

int check_blinded_paths(struct tlv_record const* const record)
{
  if(record->length < 2*EC_POINT_LENGTH + SCIDDIR_LENGTH + 3) return 0;
  byte nhops;
  byte h;
  u16 dlen;
  u64 pos;
 
  if(record->value[0] < 2) {

    if(!check_ec_point_raw(record->value + SCIDDIR_LENGTH)) return 0;
    pos = SCIDDIR_LENGTH + EC_POINT_LENGTH;

  } else {

    if(!check_ec_point_raw(record->value) || !check_ec_point_raw(record->value + EC_POINT_LENGTH)) return 0;
    pos = 2*EC_POINT_LENGTH;
  }

  while(pos + 1 < record->length) {
    nhops = record->value[pos];

    if(nhops == 0) return 0; 
    ++pos;

    for(h=0; h<nhops; ++h) {

      if(!check_ec_point_raw(record->value + pos)) return 0;
      pos += EC_POINT_LENGTH;

      if(record->length < pos + 2) return 0;
      dlen = bebuf16toh(record->value + pos);
      pos += 2 + dlen;

      if(record->length < pos) return 0;
    }

    if(pos == record->length) return 1;
    pos += (record->value[pos] < 2 ? SCIDDIR_LENGTH + EC_POINT_LENGTH : 2*EC_POINT_LENGTH);
  }
  return 0;
}
