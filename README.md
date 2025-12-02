# Bolt12 Offer Decoding Library

## Introduction
Light, fast and simple to use library to decode Bolt12 offer strings and encode the fields into JSON.

The goal of this library is to decode the Bolt12 offer fields from a Bech32 offer string as presented in [12-offer-encoding.md](https://github.com/lightning/bolts/blob/master/12-offer-encoding.md), perform some validation and encode the fields into JSON so the information can be used by a wallet app to present to the user, without the need for decoding the offer string through a Lightning node. The library is implemented in C, with bindings for Rust, Python and WASM.

## Offer Validation
The decoder is meant to perform all verifications for the reader of an offer as documented in [12-offer-encoding.md](https://github.com/lightning/bolts/blob/master/12-offer-encoding.md), except for the following: 
- `offer_features`: The field is decoded and provided in the output, but as the
library is not a lightning node and does not know about which bits are unknown,
it does not further analyze them.
- `offer_chains`: As the library is not a lightning node and does not know which
chains are known or not, it does not analyze them further.
- `offer_absolute_expiry`: The library does not compare the decoded time to the
current time.
- Elliptic curve `point` (`offer_issuer_id`, `first_node_id`, `first_path_key`, `blinded_node_id`): The library verifies that a compressed public key is provided through the number of bytes (33) and the first byte (`0x02` or `0x03`), but it does not verify that the coordinate falls on the curve, for the sake of avoiding extra dependencies. As the library is not a lightning node, if this offer was to be sent to obtain an invoice, it would be done through a lightning node which will fully validate these keys before sending an invoice request. Typically for this library, one would only be interested to compare `offer_issuer_id`, and also possibly the `first_node_id`, to expected values. These comparisons would not require EC point validation. If EC point validation is nonetheless required, it can be done externally through [libsecp256k1](https://github.com/bitcoin-core/secp256k1).

## Sample Usage Code
Sample usage C code for the library is available in [src/bin](https://github.com/21M4TW/b12od/tree/main/src/bin). Refer to the documentation in the [rust](https://github.com/21M4TW/b12od/tree/main/rust), [python](https://github.com/21M4TW/b12od/tree/main/python) and [wasm](https://github.com/21M4TW/b12od/tree/main/wasm) binding directories for related sample code.

## Example

Decoding an offer with mixed types of blinded paths, using the `b12od` C sample executable:
```
% b12od lno1pgx9getnwss8vetrw3hhyucsl5qj5qeyv5l2cs6y3qqzesrth7mlzrlp3xg7xhulusczm04x6g6nms9trspqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqqsqqqqqqqqqqqqqqqqqqqqqqqqqqpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqsqpqg3zyg3zyg3zygpqqqqzqqqqgqqxqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqqgqqqqqqqqqqqqqqqqqqqqqqqqqqqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgqqsg3zyg3zyg3zygtzzqhwcuj966ma9n9nqwqtl032xeyv6755yeflt235pmww58egx6rxry | jq
{
  "offer_description": "Test vectors",
  "offer_paths": [
    {
      "first_node_id": "0324653eac434488002cc06bbfb7f10fe18991e35f9fe4302dbea6d2353dc0ab1c",
      "first_path_key": "020202020202020202020202020202020202020202020202020202020202020202",
      "path": [
        {
          "blinded_node_id": "020202020202020202020202020202020202020202020202020202020202020202",
          "encrypted_recipient_data": "00000000000000000000000000000000"
        },
        {
          "blinded_node_id": "020202020202020202020202020202020202020202020202020202020202020202",
          "encrypted_recipient_data": "1111111111111111"
        }
      ]
    },
    {
      "first_scid": "1x2x3",
      "first_scid_dir": 1,
      "first_path_key": "020202020202020202020202020202020202020202020202020202020202020202",
      "path": [
        {
          "blinded_node_id": "020202020202020202020202020202020202020202020202020202020202020202",
          "encrypted_recipient_data": "00000000000000000000000000000000"
        },
        {
          "blinded_node_id": "020202020202020202020202020202020202020202020202020202020202020202",
          "encrypted_recipient_data": "2222222222222222"
        }
      ]
    }
  ],
  "offer_issuer_id": "02eec7245d6b7d2ccb30380bfbe2a3648cd7a942653f5aa340edcea1f283686619"
}
```
