// Helper: write JS string into WASM memory
function writeString(Module, str) {
  const encoder = new TextEncoder();
  const bytes = encoder.encode(str + "\0");
  const ptr = 0;
  new Uint8Array(Module.HEAPU8.buffer, ptr, bytes.length).set(bytes);
  return ptr;
}

// Helper: read C string from WASM memory
function readString(Module, ptr) {
  const mem = Module.HEAPU8;
  let end = ptr;
  while (mem[end] !== 0) end++;
  return new TextDecoder().decode(mem.subarray(ptr, end));
}

function Bolt12OfferDecode(Module, offer_string) {
  const b12j = Module._bolt12_offer_json_new();
  const inputPtr = writeString(Module, offer_string);
  const resultPtr = Module._bolt12_json(b12j, inputPtr);
  result = readString(Module, resultPtr);
  Module._bolt12_offer_json_delete(b12j);
  return result;
}
