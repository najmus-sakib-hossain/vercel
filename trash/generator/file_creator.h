#ifndef FILE_CREATOR_H
#define FILE_CREATOR_H

// We declare the function that Rust will be able to call.
// It takes the directory name, number of files, and content as arguments,
// and it returns the time taken in milliseconds as a double.
double create_files_in_c(const char* dir_name, int file_count, const char* content);

#endif // FILE_CREATOR_H
