use std::ffi::CString;
use std::os::raw::{c_char, c_double, c_int};
use std::time::Instant;

#[link(name = "file_creator_lib", kind = "static")]
extern "C" {
    fn create_files_in_c(dir_name: *const c_char, file_count: c_int, content: *const c_char) -> c_double;
}

fn main() {
    let rust_start_time = Instant::now();

    let dir_name = "c_modules";
    let file_count = 100;
    let content = "Hello, manfromexistence!";

    let c_dir_name = CString::new(dir_name).expect("CString::new failed for dir_name");
    let c_content = CString::new(content).expect("CString::new failed for content");

    let preparation_duration = rust_start_time.elapsed();
    println!("Handing control over to C to create {} files...", file_count);

    let time_taken_by_c = unsafe {
        create_files_in_c(c_dir_name.as_ptr(), file_count as c_int, c_content.as_ptr())
    };

    let total_rust_duration = rust_start_time.elapsed();

    if time_taken_by_c < 0.0 {
        println!("The C function reported an error.");
    } else {
        let ffi_overhead = total_rust_duration.as_micros() as f64 / 1000.0 - time_taken_by_c;

        println!("\nRust has regained control!");
        println!("Successfully created {} files in the '{}' directory.", file_count, dir_name);
        println!("\n--- Timing Breakdown ---");
        println!("1. Rust data preparation:       {:.4}ms", preparation_duration.as_micros() as f64 / 1000.0);
        println!("2. C function execution:       {:.4}ms (as reported by C)", time_taken_by_c);
        println!("3. Total time (Rust's view):   {:.4}ms", total_rust_duration.as_micros() as f64 / 1000.0);
        println!("------------------------------------");
        println!("   FFI call overhead:           ~{:.4}ms (Total - C execution)", ffi_overhead.max(0.0));
    }
}
