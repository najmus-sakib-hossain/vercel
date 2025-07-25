const std = @import("std");
const fs = std.fs;
const mem = std.mem;
const Allocator = mem.Allocator;

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const target_dirs = [_][]const u8{ "app", "components" };
    const output_dir = "components/icons";

    // Ensure output directory exists
    std.debug.print("Creating output directory: {s}\n", .{output_dir});
    try fs.cwd().makePath(output_dir);

    for (target_dirs) |dir| {
        std.debug.print("Processing directory: {s}\n", .{dir});
        processDirectory(allocator, dir, output_dir) catch |err| switch (err) {
            error.FileNotFound => {
                std.debug.print("Directory '{s}' not found, skipping...\n", .{dir});
                continue;
            },
            else => return err,
        };
    }
}

fn processDirectory(allocator: Allocator, dir_path: []const u8, output_dir: []const u8) !void {
    var dir = try fs.cwd().openDir(dir_path, .{ .iterate = true });
    defer dir.close();

    var iterator = dir.iterate();
    while (try iterator.next()) |entry| {
        if (entry.kind == .directory) {
            const sub_path = try fs.path.join(allocator, &[_][]const u8{ dir_path, entry.name });
            defer allocator.free(sub_path);
            std.debug.print("Entering subdirectory: {s}\n", .{sub_path});
            try processDirectory(allocator, sub_path, output_dir);
        } else if (entry.kind == .file and mem.endsWith(u8, entry.name, ".tsx")) {
            std.debug.print("Found .tsx file: {s}/{s}\n", .{dir_path, entry.name});
            try processTsxFile(allocator, dir_path, entry.name, output_dir);
        }
    }
}

fn processTsxFile(allocator: Allocator, dir_path: []const u8, file_name: []const u8, output_dir: []const u8) !void {
    const file_path = try fs.path.join(allocator, &[_][]const u8{ dir_path, file_name });
    defer allocator.free(file_path);

    const file = try fs.cwd().openFile(file_path, .{});
    defer file.close();

    const content = try file.readToEndAlloc(allocator, 1024 * 1024); // 1MB max
    defer allocator.free(content);

    // Check for the specific import
    const import_pattern = "import * Dx from \"@/components/icons.tsx\"";
    if (mem.containsAtLeast(u8, content, 1, import_pattern)) {
        std.debug.print("Found import in {s}\n", .{file_path});
        // Look for Dx.[IconName] pattern using a more robust approach
        var pos: usize = 0;
        while (mem.indexOf(u8, content[pos..], "<Dx.") orelse null) |start_idx| {
            pos += start_idx;
            const end_idx = mem.indexOf(u8, content[pos..], "/>") orelse break;
            const token = content[pos .. pos + end_idx + 2];
            if (mem.startsWith(u8, token, "<Dx.") and mem.endsWith(u8, token, "/>")) {
                const icon_name = token[4 .. token.len - 2]; // Extract IconName
                std.debug.print("Found icon: {s} in {s}\n", .{icon_name, file_path});
                try createIconFile(allocator, output_dir, icon_name);
            }
            pos += end_idx + 2;
        }
    } else {
        std.debug.print("No matching import found in {s}\n", .{file_path});
    }
}

fn createIconFile(allocator: Allocator, output_dir: []const u8, icon_name: []const u8) !void {
    const file_name = try std.fmt.allocPrint(allocator, "{s}.tsx", .{icon_name});
    defer allocator.free(file_name);

    const file_path = try fs.path.join(allocator, &[_][]const u8{ output_dir, file_name });
    defer allocator.free(file_path);

    std.debug.print("Creating icon file: {s}\n", .{file_path});

    const content = try std.fmt.allocPrint(allocator,
        \\import React from 'react';
        \\
        \\const {s} = () => {{
        \\    return (
        \\        <svg width="24" height="24" viewBox="0 0 24 24" fill="none" xmlns="http://www.w3.org/2000/svg">
        \\            {{/* TODO: Add actual SVG path for {s} icon */}}
        \\        </svg>
        \\    );
        \\}};
        \\
        \\export default {s};
        \\
    , .{icon_name, icon_name, icon_name});
    defer allocator.free(content);

    const file = try fs.cwd().createFile(file_path, .{});
    defer file.close();

    try file.writeAll(content);
}