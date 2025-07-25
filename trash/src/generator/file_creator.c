#include "file_creator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

// This is the implementation of the function we declared in the header.
// It contains the same logic your original main function had.
double create_files_in_c(const char* dir_name, int file_count, const char* content) {
    // Note: The C standard library `mkdir` is from POSIX and works on Linux/macOS.
    // For Windows, you might need to use `_mkdir`. The `cc` crate can handle
    // platform differences, but the C code itself needs to be compatible.
    mkdir(dir_name, 0777);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < file_count; i++) {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "%s/file_%d.txt", dir_name, i);

        FILE* file = fopen(filepath, "w");
        if (file == NULL) {
            // In a library, it's better to return an error code than to exit.
            // Here we return -1.0 to indicate failure.
            return -1.0;
        }
        fprintf(file, "%s", content);
        fclose(file);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate elapsed time in milliseconds and return it.
    double elapsed_s = (end.tv_sec - start.tv_sec);
    double elapsed_ns = (end.tv_nsec - start.tv_nsec);
    double elapsed_ms = (elapsed_s * 1000.0) + (elapsed_ns / 1000000.0);

    return elapsed_ms;
}
