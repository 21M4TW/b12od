use libc::c_char;
use std::ffi::{CStr, CString};

#[repr(C)]
pub struct bolt12_json {
    _private: [u8; 0], // opaque
}

unsafe extern "C" {
    fn bolt12_offer_json_new() -> *mut bolt12_json;
    fn bolt12_offer_json_delete(g: *mut bolt12_json);
    fn bolt12_json(g: *mut bolt12_json, offer_string: *const c_char) -> *const c_char;
}

pub struct OfferDecoder {
    ptr: *mut bolt12_json,
}

impl OfferDecoder {
    pub fn new() -> Self {
        let ptr = unsafe { bolt12_offer_json_new() };
        OfferDecoder { ptr }
    }

    pub fn decode(&self, offer_string: &str) -> String {
        let cname = CString::new(offer_string).unwrap();
        unsafe {
            let raw = bolt12_json(self.ptr, cname.as_ptr());
            CStr::from_ptr(raw).to_string_lossy().into_owned()
        }
    }
}

impl Drop for OfferDecoder {
    fn drop(&mut self) {
        unsafe { bolt12_offer_json_delete(self.ptr) };
    }
}

#[cfg(test)]
mod tests {
  use super::*; // bring the function into scope

  #[test]
  fn test_offer_decode() {
    let offer_decoder = OfferDecoder::new();
    let result = offer_decoder.decode("lno1pgx9getnwss8vetrw3hhyucsl5qj5qeyv5l2cs6y3qqzesrth7mlzrlp3xg7xhulusczm04x6g6nms9trspqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqqsqqqqqqqqqqqqqqqqqqqqqqqqqqpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqsqpqg3zyg3zyg3zygpqqqqzqqqqgqqxqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqqgqqqqqqqqqqqqqqqqqqqqqqqqqqqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgpqyqszqgqqsg3zyg3zyg3zygtzzqhwcuj966ma9n9nqwqtl032xeyv6755yeflt235pmww58egx6rxry");
    let expected = "{\"offer_description\":\"Test vectors\",\"offer_paths\":[{\"first_node_id\":\"0324653eac434488002cc06bbfb7f10fe18991e35f9fe4302dbea6d2353dc0ab1c\",\"first_path_key\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"path\":[{\"blinded_node_id\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"encrypted_recipient_data\":\"00000000000000000000000000000000\"},{\"blinded_node_id\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"encrypted_recipient_data\":\"1111111111111111\"}]},{\"first_scid\":\"1x2x3\",\"first_scid_dir\":1,\"first_path_key\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"path\":[{\"blinded_node_id\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"encrypted_recipient_data\":\"00000000000000000000000000000000\"},{\"blinded_node_id\":\"020202020202020202020202020202020202020202020202020202020202020202\",\"encrypted_recipient_data\":\"2222222222222222\"}]}],\"offer_issuer_id\":\"02eec7245d6b7d2ccb30380bfbe2a3648cd7a942653f5aa340edcea1f283686619\"}";
    assert_eq!(result, expected);
  }
}
