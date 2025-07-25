use std::fs::{create_dir, OpenOptions};
use std::io::{BufWriter, Write};
use std::path::Path;
use std::time::Instant;
use rayon::prelude::*;

fn precreate_files(paths: &[String]) {
    paths.par_iter().for_each(|path| {
        OpenOptions::new()
            .write(true)
            .create(true)
            .truncate(true)
            .open(path)
            .expect("Failed to precreate file");
    });
}

fn write_files(paths: &[String], data: &[u8]) {
    paths.par_iter().for_each(|path| {
        let file = OpenOptions::new()
            .write(true)
            .open(path)
            .expect("Failed to open file for writing");
        let mut writer = BufWriter::new(file);
        writer.write_all(data).expect("Failed to write to file");
    });
}

fn create_and_write_files_parallel(paths: &[String], data: &[u8]) {
    paths.par_iter().for_each(|path| {
        let file = OpenOptions::new()
            .create(true)
            .write(true)
            .truncate(true)
            .open(path)
            .expect("Failed to open or create file");
        let mut writer = BufWriter::new(file);
        writer.write_all(data).expect("Failed to write to file");
    });
}

fn main() {
    let start = Instant::now();
    let dir_path = "rust_modules";
    let data = b"Hello, manfromexistence";
    let paths: Vec<String> = (0..1000)
        .map(|i| format!("{}/f{}.txt", dir_path, i))
        .collect();

    if Path::new(dir_path).exists() {
        println!("Directory found. Using fast update strategy (two-pass).");
        precreate_files(&paths);
        write_files(&paths, data);
    } else {
        println!("Directory not found. Using fast creation strategy (single-pass).");
        create_dir(dir_path).expect("Failed to create directory");
        create_and_write_files_parallel(&paths, data);
    }

    let duration = start.elapsed();
    let time_ms = duration.as_secs_f64() * 1000.0;
    println!("Time taken: {:.3} ms", time_ms);
}
