fn main() {
    // Tell Cargo to rerun if CMakeLists.txt changes
    println!("cargo:rerun-if-changed=../CMakeLists.txt");

    // Run CMake to build the C library
    let dst = cmake::Config::new("..")
        .out_dir("..")
        .build_target("libb12od")
        .build();

    // Link the built library
    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=static=b12od"); // or "dylib=b12od" if shared

    // Tell Cargo to rerun if headers change
    println!("cargo:rerun-if-changed=../src/lib");
}
