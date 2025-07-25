fn main() {
    cc::Build::new()
        .file("src/generator/file_creator.c")
        .compile("file_creator_lib");
    println!("cargo:rerun-if-changed=src/generator/file_creator.c");
    println!("cargo:rerun-if-changed=src/generator/file_creator.h");
}
