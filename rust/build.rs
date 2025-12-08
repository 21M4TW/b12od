use std::env;
use std::path::PathBuf;

fn main() {
    // Tell Cargo to rerun if CMakeLists.txt changes
    println!("cargo:rerun-if-changed=./b12od-core/CMakeLists.txt");

    let abs_out = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap())
        .join("b12od-core");

    // Run CMake to build the C library
    cmake::Config::new("./b12od-core/CMakeLists.txt")
        .out_dir(&abs_out)
        .build_target("libb12od")
        .build();

    // Link the built library
    println!("cargo:rustc-link-search=native={}/build", abs_out.display());
    println!("cargo:rustc-link-lib=static=b12od"); // or "dylib=b12od" if shared

    // Tell Cargo to rerun if headers change
    println!("cargo:rerun-if-changed=./b12od-core/include");
    println!("cargo:rerun-if-changed=./b12od-core/src/lib");
}
