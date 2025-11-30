cdef extern from "b12od/bolt12_offer_json.h":
    cdef struct bolt12_json
    bolt12_json* bolt12_offer_json_new()
    const char* bolt12_offer_json(bolt12_json* handle, const char* offer_string)
    void bolt12_offer_json_delete(bolt12_json* handle)

cdef class Bolt12OfferDecoder:
    cdef bolt12_json* b12j

    def __cinit__(self):
        self.b12j = bolt12_offer_json_new()
        if self.b12j == NULL:
            raise MemoryError("Failed to create handle")

    def __dealloc__(self):
        if self.b12j != NULL:
            bolt12_offer_json_delete(self.b12j)
            self.b12j = NULL

    def decode(self, offer_string: str) -> str:
        boffer_string = offer_string.encode("utf-8")
        cdef const char* coffer_string = boffer_string
        return (<bytes>bolt12_offer_json(self.b12j, coffer_string)).decode("utf-8")

