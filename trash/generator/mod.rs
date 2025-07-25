#[link(name = "generator", kind = "static")]
extern "C" {
    fn process_tsx_file(path_ptr: *const u8, path_len: usize) -> bool;
}

pub fn process_file(path_str: &str) -> bool {
    unsafe {
        process_tsx_file(path_str.as_ptr(), path_str.len())
    }
}