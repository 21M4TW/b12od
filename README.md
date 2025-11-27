# Bolt12 Offer Decoding Library

Light and fast library to decode Bolt12 offer strings and encode them into JSON. The goal of this library is to decode the Bolt12 offer fields as presented in [12-offer-encoding.md](https://github.com/lightning/bolts/blob/master/12-offer-encoding.md), perform some validation and encode them into JSON so the information can be used by a wallet app to present it to a user, without the need for decoding it through a Lightning node. The library is implemented in C, and the plan is to offer bindings in multiple languages so it can be integrated anywhere.
