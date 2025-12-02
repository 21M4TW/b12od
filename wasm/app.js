// Helper: write JS string into WASM memory
function writeString(Module, str) {
  const encoder = new TextEncoder();
  const bytes = encoder.encode(str + "\0");
  const ptr = 0;
  new Uint8Array(Module.HEAPU8.buffer, ptr, bytes.length).set(bytes);
  return ptr;
}

// Helper: read C string from WASM memory
function readString(Module, ptr, size) {
  const mem = Module.HEAPU8;
  return new TextDecoder().decode(mem.subarray(ptr, ptr + size));
}

export function Bolt12OfferDecode(Module, offer_string) {
  const b12j = Module._bolt12_offer_json_new();
  const inputPtr = writeString(Module, offer_string);
  const resultPtr = Module._bolt12_json(b12j, inputPtr);
  const size = Module._bolt12_offer_json_get_size(b12j);
  const result = readString(Module, resultPtr, size);
  Module._bolt12_offer_json_delete(b12j);
  return result;
}

if (typeof window !== "undefined" && typeof document !== "undefined") {
// expose to global scope
window.Bolt12OfferDecode = Bolt12OfferDecode;
}
