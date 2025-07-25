const std = @import("std");
const mem = std.mem;
const fs = std.fs;
const fmt = std.fmt;

/// # DX Component Generator (Smarter Parser)
///
/// This function now correctly parses real-world JSX, including self-closing tags.
///
/// It performs the following steps:
/// 1. Takes a file path as input.
/// 2. Reads the entire content of the file into memory.
/// 3. Loops through the content, searching for all instances of "<Dx.".
/// 4. For each one found, it intelligently finds the end of the component name,
///    stopping at whitespace, '>', or '/>'.
/// 5. It creates a new file named "{IconName}.tsx".
/// 6. It writes a boilerplate React component into the new file.
///
/// @param allocator The memory allocator to use for file operations.
/// @param file_path The path to the .tsx file to process.
/// @returns `true` if at least one component was generated, `false` otherwise or on error.
fn generate_component_from_file(allocator: mem.Allocator, file_path: []const u8) !bool {
    const file_content = try fs.cwd().readFileAlloc(allocator, file_path, 1_000_000); // Limit to 1MB
    defer allocator.free(file_content);

    const start_marker = "<Dx.";
    var generated_something = false;
    var search_offset: usize = 0;

    while (search_offset < file_content.len) {
        const found_start = mem.indexOf(u8, file_content[search_offset..], start_marker);

        if (found_start) |start_index| {
            const absolute_start_index = search_offset + start_index + start_marker.len;
            const rest_of_content = file_content[absolute_start_index..];

            // Find the first occurrence of a space, '>', or '/' to mark the end of the name.
            var end_index: usize = 0;
            while (end_index < rest_of_content.len) : (end_index += 1) {
                const char = rest_of_content[end_index];
                if (char == ' ' or char == '>' or char == '/') {
                    break;
                }
            }

            if (end_index > 0) {
                const icon_name = rest_of_content[0..end_index];

                // Create the new filename.
                const new_filename = try fmt.allocPrint(allocator, "{s}.tsx", .{icon_name});
                defer allocator.free(new_filename);

                std.debug.print("Zig: Found component <Dx.{s}>. Generating file: {s}\n", .{ icon_name, new_filename });

                const new_file = try fs.cwd().createFile(new_filename, .{});
                defer new_file.close();

                const component_template =
                    \\import React from 'react';
                    \\
                    \\const {s} = () => {{{{
                    \\  return (
                    \\    <div>
                    \\      {{/* TODO: Implement the {s} icon */}}
                    \\      <span>{{{s}}}</span>
                    \\    </div>
                    \\  );
                    \\}}}};
                    \\
                    \\export default {s};
                    \\
                ;

                try new_file.writer().print(component_template, .{ icon_name, icon_name, icon_name, icon_name });
                generated_something = true;
            }

            search_offset = absolute_start_index + end_index;
        } else {
            break;
        }
    }

    return generated_something;
}

/// # FFI Export Function
///
/// This is the C-compatible wrapper function that Rust will call.
export fn process_tsx_file(path_ptr: [*c]const u8, path_len: usize) bool {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    defer _ = gpa.deinit();

    const file_path = path_ptr[0..path_len];

    const result = generate_component_from_file(allocator, file_path) catch |err| {
        std.debug.print("Zig Error: Could not process file '{s}': {s}\n", .{ file_path, @errorName(err) });
        return false;
    };

    return result;
}
