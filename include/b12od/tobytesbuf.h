#ifndef _TOBYTESBUF_
#define _TOBYTESBUF_

#include<stdint.h> 
#include<inttypes.h> 
#include<stdlib.h> 
#include<string.h> 

#include <b12od/math_utils.h>
#include <b12od/bebuf.h>
#include <b12od/hex_enc.h>

struct bytesbuf
{
  uint8_t* buf;
  size_t size;
  size_t alloc;
  float grow_fact;
};

inline static void tobb_init(struct bytesbuf* const bb)
{
  memset(bb, 0, sizeof(struct bytesbuf));
  bb->grow_fact = 1;
}

inline static void tobb_grow_fact(struct bytesbuf* const bb, const float grow_fact)
{
  if(grow_fact < 1 || grow_fact > 2) bb->grow_fact = 1;
  else bb->grow_fact = grow_fact;
}

inline static int tobb_reserve(struct bytesbuf* const bb, const size_t alloc)
{
  if(alloc > bb->alloc) {
    uint8_t* newbuf = (uint8_t*)realloc(bb->buf, alloc);

    if(!newbuf) {

      if(bb->buf) {
	free(bb->buf);
	bb->buf = NULL;
      }
      return INT32_MIN;
    }
    bb->buf = newbuf;
    bb->alloc = alloc;
  }
  return 0;
}

inline static int tobb_reserve_for(struct bytesbuf* const bb, const size_t extra_alloc)
{
#define _TOBB_RESERVE_FOR_(XTRA) {\
  size_t needed_alloc = bb->size + (XTRA); \
 \
  if(needed_alloc > bb->alloc) { \
    size_t target_alloc = ceil_u64(needed_alloc * bb->grow_fact); \
    uint8_t* newbuf = (uint8_t*)realloc(bb->buf, target_alloc); \
 \
    if(!newbuf) {\
      newbuf = (uint8_t*)realloc(bb->buf, needed_alloc); \
 \
      if(!newbuf) {\
 \
	if(bb->buf) {\
	  free(bb->buf); \
	  bb->buf = NULL; \
	} \
	return INT32_MIN; \
      }\
      bb->buf = newbuf; \
      bb->alloc = needed_alloc; \
 \
    } else {\
      bb->buf = newbuf; \
      bb->alloc = target_alloc; \
    }\
  }\
}
  _TOBB_RESERVE_FOR_(extra_alloc);
  return 0;
}

#define _tobb_noalloc(bb, bytes, nbytes) {\
  memcpy((bb)->buf + (bb)->size, (bytes), (nbytes)); \
  (bb)->size += (nbytes); \
}

inline static int tobb(struct bytesbuf* const bb, uint8_t const* const bytes, const size_t nbytes)
{
  _TOBB_RESERVE_FOR_(nbytes);
  _tobb_noalloc(bb, bytes, nbytes);
  return nbytes;
}

#define _tobb8_noalloc(bb, value) {\
  (bb)->buf[((bb)->size)++] = (value); \
}

inline static int tobb8(struct bytesbuf* const bb, uint8_t value)
{
  _TOBB_RESERVE_FOR_(1);
  _tobb8_noalloc(bb, value);
  return 1;
}

#define _tobb16_noalloc(bb, value) {\
  htobebuf16((value), (bb)->buf + (bb)->size); \
  (bb)->size += 2; \
}

inline static int tobb16(struct bytesbuf* const bb, uint16_t value)
{
  _TOBB_RESERVE_FOR_(2);
  _tobb16_noalloc(bb, value);
  return 2;
}

#define _tobb32_noalloc(bb, value) {\
  htobebuf32((value), (bb)->buf + (bb)->size); \
  (bb)->size += 4; \
}

inline static int tobb32(struct bytesbuf* const bb, uint32_t value)
{
  _TOBB_RESERVE_FOR_(4);
  _tobb32_noalloc(bb, value);
  return 4;
}

#define _tobb64_noalloc(bb, value) {\
  htobebuf64((value), (bb)->buf + (bb)->size); \
  (bb)->size += 8; \
}

inline static int tobb64(struct bytesbuf* const bb, uint64_t value)
{
  _TOBB_RESERVE_FOR_(8);
  _tobb64_noalloc(bb, value);
  return 8;
}

#define _tobb_hex_enc_noalloc(bb, bytes, nbytes) {\
  size_t b; \
 \
  for(b=0; b<(nbytes); ++b) hex8((bytes)[b], (char*)((bb)->buf + (bb)->size + 2*b)); \
  (bb)->size += 2*(nbytes); \
}

inline static int tobb_hex_enc(struct bytesbuf* const bb, uint8_t const* const bytes, const size_t nbytes)
{
  _TOBB_RESERVE_FOR_(2*nbytes);
  _tobb_hex_enc_noalloc(bb, bytes, nbytes);
  return nbytes;
}

#define _tobb_trunc_hex_enc_noalloc(bb, bytes, nbytes) {\
  size_t b; \
  size_t h=0; \
 \
  for(b=0; b<(nbytes); ++b) \
 \
    if((bytes)[b]) { \
      hex8((bytes)[b], (char*)((bb)->buf + (bb)->size + h)); \
      h += 2; \
    } \
  (bb)->size += h; \
}

inline static int tobb_trunc_hex_enc(struct bytesbuf* const bb, uint8_t const* const bytes, const size_t nbytes)
{
  _TOBB_RESERVE_FOR_(2*nbytes);
  const size_t init_size = bb->size;
  _tobb_trunc_hex_enc_noalloc(bb, bytes, nbytes);
  return bb->size - init_size;
}

#define tobb_str(bb, string) tobb(bb, string, strlen(string))
#define _tobb_str_noalloc(bb, string) _tobb_noalloc(bb, string, strlen(string))

inline static void tobb_free(struct bytesbuf* const bb){if(bb->buf) {free(bb->buf); bb->buf = NULL; bb->alloc = 0; bb->size = 0;}}

inline static void tobb_reset(struct bytesbuf* const bb){bb->size = 0;}

inline static void tobb_shrink_to_fit(struct bytesbuf* const bb){
  if(bb->size > 0) {
    uint8_t* newbuf = (uint8_t*)realloc(bb->buf, bb->size);

    if(newbuf) {
      bb->buf = newbuf;
      bb->alloc = bb->size;
    }

  } else {
    free(bb->buf);
    bb->buf = NULL;
  }
}

#endif
