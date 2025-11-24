#ifndef _BOLT12_TLV_FIELD_CHECK_
#define _BOLT12_TLV_FIELD_CHECK_

#include "tlv_field_check.h"

inline static int check_offer_chains(struct tlv_record const* const record){return ((record->length%CHAIN_HASH_LENGTH) == 0);}

inline static int check_offer_currency(struct tlv_record const* const record){return (record->length == 3);}

#define check_offer_amount (check_tu64)

#define check_offer_absolute_expiry (check_tu64)

#define check_offer_quantity_max (check_tu64)

#define check_offer_issuer_id (check_ec_point)

int check_offer_paths(struct tlv_record const* const record);

#endif
