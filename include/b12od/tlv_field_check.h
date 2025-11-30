#ifndef _TLV_FIELD_CHECK_
#define _TLV_FIELD_CHECK_

#include <b12od/bolt12_types.h>
#include <b12od/tlv.h>

inline static int check_u64(struct tlv_record const* const record){return (record->length == sizeof(u64));}
inline static int check_tu64(struct tlv_record const* const record){return (record->length <= sizeof(u64) && (record->length == 0 || record->value[0] != 0));}

inline static int check_chain_hash(struct tlv_record const* const record){return (record->length == CHAIN_HASH_LENGTH);}
inline static int check_channel_id(struct tlv_record const* const record){return (record->length == CHANNEL_ID_LENGTH);}
inline static int check_sha256(struct tlv_record const* const record){return (record->length == SHA256_LENGTH);}
inline static int check_signature(struct tlv_record const* const record){return (record->length == SIGNATURE_LENGTH);}
inline static int check_bip340sig(struct tlv_record const* const record){return (record->length == BIP340SIG_LENGTH);}
inline static int check_ec_point_raw(byte const* const data){return (data[0] == 0x02 || data[0] == 0x03);}
inline static int check_ec_point(struct tlv_record const* const record){return (record->length == EC_POINT_LENGTH && check_ec_point_raw(record->value));}
inline static int check_short_channel_id(struct tlv_record const* const record){return (record->length == SHORT_CHANNEL_ID_LENGTH);}

#endif
