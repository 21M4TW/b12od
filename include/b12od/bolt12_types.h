#ifndef _BOLT12_TYPES_
#define _BOLT12_TYPES_

#include<stdint.h>

typedef uint8_t  byte;
typedef int8_t   s8  ;
typedef uint16_t u16 ;
typedef int16_t  s16 ;
typedef uint32_t u32 ;
typedef int32_t  s32 ;
typedef uint64_t u64 ;
typedef int64_t  s64 ;

typedef u64 bigsize;
typedef byte utf8;

enum eBolt12Types{
  TYPE_OFFER_CHAINS=		2,
  TYPE_OFFER_METADATA=		4,
  TYPE_OFFER_CURRENCY=		6,
  TYPE_OFFER_AMOUNT=		8,
  TYPE_OFFER_DESCRIPTION=	10,
  TYPE_OFFER_FEATURES=		12,
  TYPE_OFFER_ABSOLUTE_EXPIRY=	14,
  TYPE_OFFER_PATHS=		16,
  TYPE_OFFER_ISSUER=		18,
  TYPE_OFFER_QUANTITY_MAX=	20,
  TYPE_OFFER_ISSUER_ID=		22,
};

#define CHAIN_HASH_LENGTH (32)
#define CHANNEL_ID_LENGTH (32)
#define SHA256_LENGTH (32)
#define SIGNATURE_LENGTH (64)
#define BIP340SIG_LENGTH (64)
#define SCIDDIR_LENGTH (9)
#define EC_POINT_LENGTH (33)
#define SHORT_CHANNEL_ID_LENGTH (8)

#endif
