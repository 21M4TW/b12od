#include <b12od/bolt12_json_tlv.h>

inline static int sciddir_or_pubkey_enc_func_noalloc(uint8_t const* const data, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;
  uint64_t sciddir;

  //SCIDDIR
  if(data[0] < 2) {
    sciddir = bebuf64toh(data + 1);
    if((ret=sprintf((char*)bb->buf + bb->size, "\"first_scid\":\"%"PRIu32"x%"PRIu32"x%"PRIu16"\",\"first_scid_dir\":%"PRIu8, (uint32_t)(sciddir>>40), (uint32_t)((sciddir>>16)&0xffffff), (uint16_t)(sciddir&0xffff), data[0])) < 0) return ret;
    bb->size += ret;
    return SCIDDIR_LENGTH;

  //Pub key
  } else {
    json_add_name_value_noalloc("first_node_id", hex_string, data, EC_POINT_LENGTH, bb);
    return EC_POINT_LENGTH;
  }
}

int blinded_path_hop_value_func(uint8_t const* data, size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;
  size_t rdlen = bebuf16toh(data + EC_POINT_LENGTH);

  if((ret=tobb_reserve_for(bb, 1 /* '{' */ +
			       json_name_value_maxlength("blinded_node_id", hex_string, EC_POINT_LENGTH) + /* Includes extra byte for ',' */
			       json_name_value_maxlength("encrypted_recipient_data", string, rdlen) + /* Includes extra byte for possible ',' after '}' */
			       1 /* '}' */
			       ))) return ret;
  _tobb8_noalloc(bb, '{');
  json_add_name_value_noalloc("blinded_node_id", hex_string, data, EC_POINT_LENGTH, bb);
  _tobb8_noalloc(bb, ',');
  json_add_name_value_noalloc("encrypted_recipient_data", hex_string, data + EC_POINT_LENGTH + 2, rdlen, bb);
  _tobb8_noalloc(bb, '}');
  return EC_POINT_LENGTH + 2 + rdlen;
}

int blinded_path_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;
  byte nhops;
  byte i;
  uint8_t const* dataptr = data;

  if((ret=tobb_reserve_for(bb, 1 /* '{' */ +
	  		       json_name_value_maxlength("first_node_id", hex_string, EC_POINT_LENGTH) + /* first_node_id has a longer string than first_scid + first_scid_dir */
	                       json_name_value_maxlength("first_path_key", hex_string, EC_POINT_LENGTH) +
			       json_name_length("path") +
			       1 /* "[" */ +
			       json_name_value_maxlength("blinded_node_id", hex_string, EC_POINT_LENGTH)
			       ))) return ret;

  _tobb8_noalloc(bb, '{');
  if((ret = sciddir_or_pubkey_enc_func_noalloc(dataptr, bb)) < 0) return ret;
  dataptr += ret;
  
  _tobb8_noalloc(bb, ',');
  json_add_name_value_noalloc("first_path_key", hex_string, dataptr, EC_POINT_LENGTH, bb);
  dataptr += EC_POINT_LENGTH;
  _tobb8_noalloc(bb, ',');
  json_add_name_noalloc("path", bb);
  _tobb8_noalloc(bb, '[');
  nhops = dataptr[0];
  ++dataptr;

  if((ret = blinded_path_hop_value_func(dataptr, 0, bb)) < 0) return ret;
  dataptr += ret;

  for(i = 1; i < nhops; ++i) {
    _tobb8_noalloc(bb, ',');

    if((ret = blinded_path_hop_value_func(dataptr, 0, bb)) < 0) return ret;
    dataptr += ret;
  }
  if((ret=tobb_reserve_for(bb, 3))) return ret; /* ']},' */
  _tobb8_noalloc(bb, ']');
  _tobb8_noalloc(bb, '}');

  return (dataptr - data);
}
