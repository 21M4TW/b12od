# Bolt12 Offer Decoding Library

Light, fast and simple to use library to decode Bolt12 offer strings and encode the fields into JSON.

The goal of this library is to decode the Bolt12 offer fields from a Bech32 offer string as presented in [12-offer-encoding.md](https://github.com/lightning/bolts/blob/master/12-offer-encoding.md), perform some validation and encode the fields into JSON so the information can be used by a wallet app to present to the user, without the need for decoding the offer string through a Lightning node. The library is implemented in C, with bindings for Rust, Python and WASM.
