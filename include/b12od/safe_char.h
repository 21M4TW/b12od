#ifndef _SAFE_CHAR_
#define _SAFE_CHAR_

#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#include <b12od/hex_enc.h>

//Basic flags
#define SC_AAUP	 (1)
#define SC_AALOW (1<<1)
#define SC_ANUM	 (1<<2)
#define SC_ASYM	 (1<<3)
#define SC_ASYE	 (1<<4)
#define SC_ASPA  (1<<5)
#define SC_ASPE  (1<<6)
#define SC_ACC   (1<<7)
#define SC_ACCE  (1<<8)
#define SC_U2F   (1<<9)
#define SC_U3F   (1<<10)
#define SC_U4F   (1<<11)
#define SC_UO    (1<<12)

//Combined flags
#define SC_ASPAE (SC_ASPA|SC_ASPE)
#define SC_ASPS  (SC_ASPA|SC_ASYM)
#define SC_UF    (SC_U2F|SC_U3F|SC_U4F)

//Flag masks
#define SC_AA_MASK    (SC_AAUP|SC_AALOW)
#define SC_AAN_MASK   (SC_AA_MASK|SC_ANUM)
#define SC_AANS_MASK  (SC_AAN_MASK|SC_ASYM)
#define SC_AANSE_MASK (SC_AANS_MASK|SC_ASYE)
#define SC_APRIN_MASK (SC_AANS_MASK|SC_ASPA)
#define SC_APRIE_MASK (SC_ANSE_MASK|SC_ASPAE)
#define SC_ALL        (~(uint16_t)0)

enum eSafeCharErrors{SAFE_CHAR_OK=0, SAFE_CHAR_EOF=EOF, SAFE_CHAR_INVALID_UTF8=EOF-1};

extern const uint16_t _ctable[256];
extern char const* const _etable[256];

inline static int8_t read_single_utf8(const char** inbuf, size_t* inbuflen, const uint16_t char_mask, char outbuf[6])
{
  if(*inbuflen == 0) return SAFE_CHAR_EOF;

  if((_ctable[(uint8_t)(*inbuf)[0]]&char_mask) == 0) {
    ++(*inbuf);
    --(*inbuflen);
    return 0;
  }

  //If single byte in and out
  if(_ctable[(uint8_t)(*inbuf)[0]]&SC_APRIN_MASK) {
    outbuf[0] = (*inbuf)[0];
    ++(*inbuf);
    --(*inbuflen);
    return 1;
  }

  //If is escapable with \ (not necessarily printable, single byte in, two bytes out)
  if(_etable[(uint8_t)(*inbuf)[0]]) {
    outbuf[0] = _etable[(uint8_t)(*inbuf)[0]][0];
    outbuf[1] = _etable[(uint8_t)(*inbuf)[0]][1];
    ++(*inbuf);
    --(*inbuflen);
    return 2;
  }

  //If seems to be 2-byte UTF-8 
  if(_ctable[(uint8_t)(*inbuf)[0]]&SC_U2F) {

    //If input buffer insufficient, assume it is truncated
    if(*inbuflen < 2) return SAFE_CHAR_EOF;

    //If we are not dealing with an UTF-8 control character (U+0080 - U+009F)
    if(((*inbuf)[0]&0x1) || ((*inbuf)[1]&0x20)) {

      //If 2-byte UTF-8 is confirmed
      if(_ctable[(uint8_t)(*inbuf)[1]]&SC_UO) {
	*(uint16_t*)outbuf = *(uint16_t*)(*inbuf);
	(*inbuf) += 2;
	(*inbuflen) -= 2;
	return 2;
      }

    } else return SAFE_CHAR_INVALID_UTF8;

    //Else if it seems to be 3-byte UTF-8
  } else if(_ctable[(uint8_t)(*inbuf)[0]]&SC_U3F) {

    //If input buffer insufficient, assume it is truncated
    if(*inbuflen < 3) return SAFE_CHAR_EOF;

    //If 3-byte UTF-8 is confirmed
    if((_ctable[(uint8_t)(*inbuf)[1]]&SC_UO) && (_ctable[(uint8_t)(*inbuf)[2]]&SC_UO)) {
      outbuf[0] = (*inbuf)[0];
      outbuf[1] = (*inbuf)[1];
      outbuf[2] = (*inbuf)[2];
      (*inbuf) += 3;
      (*inbuflen) -= 3;
      return 3;
    } else return SAFE_CHAR_INVALID_UTF8;

    //Else if it seems to be 4-byte UTF-8
  } else if(_ctable[(uint8_t)(*inbuf)[0]]&SC_U4F) {

    //If input buffer insufficient, assume it is truncated
    if(*inbuflen < 4) return SAFE_CHAR_EOF;

    //If 4-byte UTF-8 is confirmed
    if((_ctable[(uint8_t)(*inbuf)[1]]&SC_UO) && (_ctable[(uint8_t)(*inbuf)[2]]&SC_UO) && (_ctable[(uint8_t)(*inbuf)[3]]&SC_UO)) {
      *(uint32_t*)outbuf = *(uint32_t*)(*inbuf);
      (*inbuf) += 4;
      (*inbuflen) -= 4;
      return 4;
    } else return SAFE_CHAR_INVALID_UTF8;

  } else if(_ctable[(uint8_t)(*inbuf)[0]]&SC_UO) return SAFE_CHAR_INVALID_UTF8;

  //Otherwise we assume unknown encoding, and we \u escape the character
  outbuf[0] = '\\';
  outbuf[1] = 'u';
  outbuf[2] = '0';
  outbuf[3] = '0';
  hex8((*inbuf)[0], outbuf+4);
  ++(*inbuf);
  --(*inbuflen);
  return 6;
}

inline static int read_utf8(const char* inbuf, size_t inbuflen, const uint16_t char_mask, char* const outbuf, const size_t outbuflen, size_t* const outlen)
{
  int8_t ret=-1;
  *outlen=0;

  if(outbuflen < 6*inbuflen) return -1;

  while(inbuflen > 0 && (ret = read_single_utf8(&inbuf, &inbuflen, char_mask, outbuf + *outlen)) >= 0) *outlen += ret;

  return ret;
}

#endif
