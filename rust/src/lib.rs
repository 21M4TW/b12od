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
