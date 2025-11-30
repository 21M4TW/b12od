#ifndef _JSON_ENCODE_
#define _JSON_ENCODE_

#include <stdio.h>
#include <inttypes.h>

#include <b12od/safe_char.h>
#include <b12od/tobytesbuf.h>

#ifndef _MINIMUM_ALLOC_TESTING_
  #define _JSON_BB_GROW_FACT (1.25)
#else
  #define _JSON_BB_GROW_FACT (1)
#endif
#define _JSON_BB_RESERVE (256) //Must be sufficient for any JSON error message

struct json
{
  struct bytesbuf bb;
};

inline static int json_init(struct json* const jctx)
{
  tobb_init(&jctx->bb);
  tobb_grow_fact(&jctx->bb, _JSON_BB_GROW_FACT);
  return 0;
}

inline static int json_error(struct json* const jctx, const int64_t error_code, const char* error_msg)
{
  tobb_reserve(&jctx->bb, _JSON_BB_RESERVE);
  int ret;
  if((ret = sprintf((char*)jctx->bb.buf, "{\"code\":%"PRIi64",\"message\":\"%s\"}", error_code, error_msg)) < 0) return ret;
  return 0;
}

#define json_name_length(name) strlen("\""name"\":")

#define json_add_name_noalloc(name, bb) _json_add_name_noalloc("\""name"\":", strlen("\""name"\":"), bb)
#define _json_add_name_noalloc(name, length, bb) {\
  _tobb_noalloc(bb, name, length); \
}

#define json_add_name_value_noalloc(name, vname, data, dlen, bb) _json_add_name_value_noalloc("\""name"\":", strlen("\""name"\":"), vname, data, dlen, bb)
#define _json_add_name_value_noalloc(name, length, vname, data, dlen, bb) {\
  _json_add_name_noalloc(name, length, bb); \
  if((ret=vname ## _value_func_noalloc(data, dlen, bb)) < 0) return ret; \
}

#define json_name_value_maxlength(name, vname, dlen) _json_name_value_maxlength(strlen("\""name"\":"), vname, dlen)
#define _json_name_value_maxlength(length, vname, dlen) ((length) + vname ## _value_maxlength(dlen))

#define json_add_name_value(jctx, name, vname, data, dlen) _json_add_name_ ## vname ## _value(jctx, "\""name"\":", strlen("\""name"\":"), data, dlen)
#define JSON_ADD_NAME_VALUE_DEF(vname) \
inline static int _json_add_name_ ## vname ## _value(struct json* const jctx, const char* name, size_t length, uint8_t const* const data, const size_t dlen) \
{ \
  int ret; \
  \
  if((ret=tobb_reserve_for(&jctx->bb, _json_name_value_maxlength(length, vname, dlen)))) return ret; \
  _json_add_name_value_noalloc(name, length, vname, data, dlen, &jctx->bb); \
  return 0; \
}

#define json_add_name_fixed_array(jctx, name, vname, data, dlen) _json_add_name_ ## vname ## _fixed_array(jctx, "\""name"\":", strlen("\""name"\":"), data, dlen)
#define JSON_ADD_NAME_FIXED_ARRAY_DEF(vname) \
inline static int _json_add_name_ ## vname ## _fixed_array(struct json* const jctx, const char* name, size_t length, uint8_t const* const data, const size_t dlen) \
{ \
  int ret; \
  const size_t nelements = dlen / vname ## _element_length; \
  size_t i; \
  \
  if((ret=tobb_reserve_for(&jctx->bb, length + nelements * vname ## _value_maxlength(vname ## _element_length) + 2))) return ret; /* +2 for '[' and ']' */ \
  _json_add_name_noalloc(name, length, &jctx->bb); \
  \
  _tobb8_noalloc(&jctx->bb, '['); \
  if(nelements > 0) {\
    \
    if((ret=vname ## _value_func_noalloc(data, vname ## _element_length, &jctx->bb)) < 0) return ret; \
    \
    for(i=1; i < nelements; ++i) {\
      \
      _tobb8_noalloc(&jctx->bb, ','); \
      \
      if((ret=vname ## _value_func_noalloc(data + i * vname ## _element_length, vname ## _element_length, &jctx->bb)) < 0) return ret; \
    } \
  } \
  _tobb8_noalloc(&jctx->bb, ']'); \
  return 0; \
}

#define json_add_name_variable_array(jctx, name, vname, data, dlen) _json_add_name_ ## vname ## _variable_array(jctx, "\""name"\":", strlen("\""name"\":"), data, dlen)
#define JSON_ADD_NAME_VARIABLE_ARRAY_DEF(vname) \
inline static int _json_add_name_ ## vname ## _variable_array(struct json* const jctx, const char* name, size_t length, uint8_t const* const data, const size_t dlen) \
{ \
  int ret; \
  size_t rdlen = 0; \
  \
  if((ret=tobb_reserve_for(&jctx->bb, length+1))) return ret; /* +1 for "[" */ \
  _json_add_name_noalloc(name, length, &jctx->bb); \
  _tobb8_noalloc(&jctx->bb, '['); \
  \
  if(rdlen < dlen) {\
    \
    if((ret=vname ## _value_func(data + rdlen, dlen - rdlen, &jctx->bb)) < 0) return ret; /* vname ## _value_func must allocate for a possible extra "," */ \
    rdlen += ret; \
    \
    while(rdlen < dlen) {\
      _tobb8_noalloc(&jctx->bb, ','); \
      \
      if((ret=vname ## _value_func(data + rdlen, dlen - rdlen, &jctx->bb)) < 0) return ret; /* vname ## _value_func must allocate for a possible extra ',' */ \
      rdlen += ret; \
    } \
  } \
  \
  if((ret=tobb_reserve_for(&jctx->bb, 2))) return ret; /* +1 for a possible extra ',' */ \
  _tobb8_noalloc(&jctx->bb, ']'); \
  return 0; \
}

inline static int _tobb_utf8_enc_noalloc(struct bytesbuf *bb, uint8_t const* const bytes, const size_t nbytes, const uint16_t char_mask)
{
  size_t outlen;
  int ret = read_utf8((const char*)bytes, nbytes, char_mask, (char*)bb->buf + bb->size, bb->alloc - bb->size, &outlen);

  if(ret < 0) return ret;
  bb->size += outlen;
  return outlen;
}

inline static int string_value_func_noalloc(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  int ret;
  _tobb8_noalloc(bb, '\"');

  if((ret=_tobb_utf8_enc_noalloc(bb, data, dlen, SC_ALL)) < 0) return ret;
  _tobb8_noalloc(bb, '\"');
  return dlen;
}

#define string_value_maxlength(nbytes) ((nbytes)*6 + 3) // includes + 1 for possible ','

JSON_ADD_NAME_VALUE_DEF(string);

inline static int hex_string_value_func_noalloc(uint8_t const* const data, const size_t dlen, struct bytesbuf *bb)
{
  //Returns the number of read bytes if no error
  _tobb8_noalloc(bb, '\"');
  _tobb_hex_enc_noalloc(bb, data, dlen);
  _tobb8_noalloc(bb, '\"');
  return dlen;
}

#define hex_string_value_maxlength(nbytes) ((nbytes)*2 + 3) // includes + 1 for possible ','

JSON_ADD_NAME_VALUE_DEF(hex_string);

inline static void json_free(struct json* const jctx){tobb_free(&jctx->bb);}

inline static void json_reset(struct json* const jctx){tobb_reset(&jctx->bb);}

inline static void json_shrink_to_fit(struct json* const jctx){tobb_shrink_to_fit(&jctx->bb);}

#endif
