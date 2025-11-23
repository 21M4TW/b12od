#ifndef _JSON_ENCODE_
#define _JSON_ENCODE_

#include <stdio.h>
#include <inttypes.h>
#include "safe_char.h"
#include "tobytesbuf.h"

#define _JSON_BB_GROW_FACT (1.25)
#define _JSON_BB_RESERVE (256) //Must be sufficient for any JSON error message

struct json
{
  struct bytesbuf bb;
};

inline static int json_init(struct json* const jctx)
{
  tobb_init(&jctx->bb);
  tobb_grow_fact(&jctx->bb, _JSON_BB_GROW_FACT);
  return tobb_reserve(&jctx->bb, _JSON_BB_RESERVE);
}

inline static int json_error(struct json* const jctx, const int64_t error_code, const char* error_msg)
{
  tobb_reserve(&jctx->bb, _JSON_BB_RESERVE);
  int ret = sprintf((char*)jctx->bb.buf, "{\"code\":%"PRIi64",\"message\":\"%s\"}", error_code, error_msg);
  return 0;
}

#define json_add_name(jctx, name) _json_add_name(jctx, "\""name"\":", strlen("\""name"\":"))
#define _json_add_name_unsafe(jctx, name, length) {\
  _tobb_unsafe(&jctx->bb, name, length); \
}
inline static int _json_add_name(struct json* const jctx, const char* name, size_t length)
{
  int ret;

  if((ret=tobb_reserve_for(&jctx->bb, length+1))) return ret; //+1 for possible "," that can occur
  _json_add_name_unsafe(jctx, name, length);
  return 0;
}

#define json_add_value(jctx, vname, data, dlen) _json_add_ ## vname ## _value(jctx, data, dlen)
#define JSON_ADD_VALUE_DEF(vname) \
inline static int json_add_ ## vname ## _value(struct json* const jctx, uint8_t const* const data, const size_t dlen) \
{ \
  int ret; \
 \
  if((ret=tobb_reserve_for(&jctx->bb, vname ## _value_maxlength(dlen)+1))) return ret; /* +1 for possible "," that can occur */ \
  if((ret=vname ## _value_func(data, dlen, &jctx->bb))) return ret; \
  return 0; \
}

#define json_add_name_value(jctx, name, vname, data, dlen) _json_add_name_ ## vname ## _value(jctx, "\""name"\":", strlen("\""name"\":"), data, dlen)
#define JSON_ADD_NAME_VALUE_DEF(vname) \
inline static int _json_add_name_ ## vname ## _value(struct json* const jctx, const char* name, size_t length, uint8_t const* const data, const size_t dlen) \
{ \
  int ret; \
 \
  if((ret=tobb_reserve_for(&jctx->bb, length+vname ## _value_maxlength(dlen)+1))) return ret; /* +1 for possible "," that can occur */ \
  _json_add_name_unsafe(jctx, name, length); \
  if((ret=vname ## _value_func(data, dlen, &jctx->bb))) return ret; \
  return 0; \
}

inline static int _tobb_utf8_enc_unsafe(struct bytesbuf *bb, uint8_t const* const bytes, const size_t nbytes, const uint16_t char_mask)
{
  ssize_t ret = read_utf8((const char*)bytes, nbytes, (char*)bb->buf + bb->size, bb->alloc - bb->size, char_mask);

  if(ret < 0) return (int)ret;
  bb->size += ret;
  return 0;
}

inline static int string_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  int ret;
  _tobb8_unsafe(bb, '\"');

  if((ret=_tobb_utf8_enc_unsafe(bb, data, dlen, SC_ALL))) return ret;
  _tobb8_unsafe(bb, '\"');
  return 0;
}

#define string_value_maxlength(nbytes) ((nbytes)*6+2)

JSON_ADD_VALUE_DEF(string);
JSON_ADD_NAME_VALUE_DEF(string);

inline static int alpha_string_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  int ret;
  _tobb8_unsafe(bb, '\"');

  if((ret=_tobb_utf8_enc_unsafe(bb, data, dlen, SC_AA_MASK))) return ret;
  _tobb8_unsafe(bb, '\"');
  return 0;
}

#define alpha_string_value_maxlength(nbytes) ((nbytes)+2)

JSON_ADD_VALUE_DEF(alpha_string);
JSON_ADD_NAME_VALUE_DEF(alpha_string);

inline static int hex_string_value_func(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  int ret;
  _tobb8_unsafe(bb, '\"');
  _tobb_hex_enc_unsafe(bb, data, dlen);
  _tobb8_unsafe(bb, '\"');
  return 0;
}

#define hex_string_value_maxlength(nbytes) ((nbytes)*2+2)

JSON_ADD_VALUE_DEF(hex_string);
JSON_ADD_NAME_VALUE_DEF(hex_string);

inline static void json_free(struct json* const jctx){tobb_free(&jctx->bb);}

#endif
