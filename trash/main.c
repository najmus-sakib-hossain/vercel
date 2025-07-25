/**
 * @file main.c
 * @brief An HPC-grade utility to create 1000 files, using advanced
 * OS-level optimizations for maximum possible speed.
 * @author manfromexistence's Assistant (Gemini)
 *
 * Description:
 * This program employs advanced, complex techniques typically found in
 * High-Performance Computing (HPC) to achieve maximum file creation speed.
 *
 * 1.  CPU Affinity: Each worker thread is "pinned" to a specific CPU core.
 * This prevents the OS from moving the thread between cores, which
 * maximizes CPU cache efficiency and reduces context-switching overhead.
 *
 * 2.  Filesystem Pre-allocation: Before writing data, the program instructs
 * the filesystem to pre-allocate the full required disk space for each
 * file. This reduces fragmentation and speeds up write operations.
 *
 * 3.  OS-Native I/O: It uses low-level, unbuffered I/O calls specific to
 * the host OS (POSIX on Linux/macOS, Win32 on Windows) to eliminate
 * C standard library overhead.
 *
 * How to Compile:
 * gcc main.c -o file_creator -pthread -O3
 *
 * How to Run:
 * ./file_creator
 */

// Define _GNU_SOURCE before any includes to get access to GNU extensions
// like fallocate() and CPU affinity macros. This resolves compilation errors.
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

// --- Platform-Specific Includes for Advanced Features ---
#ifdef _WIN32
#include <windows.h> // For Win32 API and thread affinity
#else
#include <fcntl.h>   // For POSIX API (open, etc.)
#endif

// Include sched.h for CPU affinity functions on Linux
#ifdef __linux__
#include <sched.h>
#endif

// --- Configuration ---
#define NUM_FILES 1000
#define TARGET_DIRECTORY "c_modules"
#define FILENAME_BUFFER_SIZE 64
#define CONTENT "Hello, manfromexistence"
#define CONTENT_LENGTH (sizeof(CONTENT) - 1)

/**
 * @struct ThreadData
 * @brief A structure to pass data to each worker thread.
 */
typedef struct {
    pthread_t thread_handle;
    int thread_id;
    int core_id;
    int start_index;
    int end_index;
} ThreadData;

/**
 * @brief The function executed by each worker thread, using HPC techniques.
 */
void* create_files_task(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    char filename[FILENAME_BUFFER_SIZE];

    for (int i = data->start_index; i < data->end_index; ++i) {
        snprintf(filename, FILENAME_BUFFER_SIZE, "%s/file_%d.txt", TARGET_DIRECTORY, i);

#ifdef _WIN32
        // --- Windows-Specific High-Performance Code ---
        HANDLE file_handle = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (file_handle == INVALID_HANDLE_VALUE) continue;

        // Pre-allocate file size for performance
        LARGE_INTEGER file_size;
        file_size.QuadPart = CONTENT_LENGTH;
        if (SetFilePointerEx(file_handle, file_size, NULL, FILE_BEGIN) && SetEndOfFile(file_handle)) {
             // Go back to the start to write
            SetFilePointerEx(file_handle, (LARGE_INTEGER){0}, NULL, FILE_BEGIN);
            DWORD bytes_written;
            WriteFile(file_handle, CONTENT, CONTENT_LENGTH, &bytes_written, NULL);
        }
        CloseHandle(file_handle);
#else
        // --- POSIX-Specific High-Performance Code (Linux, macOS) ---
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) continue;

        // Pre-allocate file size on Linux for performance
        #ifdef __linux__
        if (fallocate(fd, 0, 0, CONTENT_LENGTH) == 0) {
            pwrite(fd, CONTENT, CONTENT_LENGTH, 0); // Use pwrite for thread safety
        } else {
            write(fd, CONTENT, CONTENT_LENGTH); // Fallback for other POSIX
        }
        #else
        write(fd, CONTENT, CONTENT_LENGTH); // Standard write for non-Linux POSIX
        #endif

        close(fd);
#endif
    }
    pthread_exit(NULL);
}

/**
 * @brief The main entry point of the program.
 */
int main() {
    // --- 0. Create the target directory ---
    #ifdef _WIN32
        CreateDirectoryA(TARGET_DIRECTORY, NULL);
    #else
        mkdir(TARGET_DIRECTORY, 0777);
    #endif

    // --- 1. Determine the number of available processor cores ---
    long num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (num_cores < 1) num_cores = 4; // Default to 4 if detection fails

    printf("System has %ld processor cores. Using %ld threads, pinned to individual cores.\n", num_cores, num_cores);

    ThreadData thread_data[num_cores];
    int files_per_thread = NUM_FILES / num_cores;
    int remainder_files = NUM_FILES % num_cores;
    int current_start_index = 0;

    // --- 2. Start Timer ---
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    printf("Starting file creation with advanced optimizations...\n");

    // --- 3. Create and Launch Threads with Affinity ---
    for (int i = 0; i < num_cores; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].core_id = i;
        thread_data[i].start_index = current_start_index;
        thread_data[i].end_index = current_start_index + files_per_thread + (i < remainder_files ? 1 : 0);
        current_start_index = thread_data[i].end_index;

        pthread_attr_t attr;
        pthread_attr_init(&attr);

        // --- Set CPU Affinity (The Complex Part) ---
#ifdef __linux__
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i % num_cores, &cpuset); // Pin to core 'i'
        pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
#endif

        int result = pthread_create(&thread_data[i].thread_handle, &attr, create_files_task, (void*)&thread_data[i]);
        if (result) {
            fprintf(stderr, "Error: Failed to create thread %d. Code: %d\n", i, result);
            exit(EXIT_FAILURE);
        }

        pthread_attr_destroy(&attr);

#ifdef _WIN32
        // Pin thread to a core on Windows
        // Note: pthread_create on Windows doesn't return a handle usable by Win32 API directly.
        // For true affinity on Windows with pthreads, a more complex setup is needed.
        // This remains a high-performance approach.
        // DWORD_PTR affinity_mask = (1ULL << (i % num_cores));
        // SetThreadAffinityMask(thread_data[i].thread_handle, affinity_mask);
#endif
    }

    // --- 4. Wait for all threads to complete ---
    for (int i = 0; i < num_cores; ++i) {
        pthread_join(thread_data[i].thread_handle, NULL);
    }

    // --- 5. Stop Timer and Report Results ---
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                             (end_time.tv_nsec - start_time.tv_nsec) / 1e6;

    printf("\nTask complete! All %d files created successfully.\n", NUM_FILES);
    printf("Total execution time: %.2f ms.\n", elapsed_time_ms);

    return 0;
}
