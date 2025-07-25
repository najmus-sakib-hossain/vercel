const std = @import("std");

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const data_dir = "ui/data/icons";
    var total_icon_count: u32 = 0;

    std.debug.print("Transformer: Scanning directory '{s}' for icon sets...\n", .{data_dir});

    const result = try std.process.Child.run(.{
        .allocator = allocator,
        .argv = &.{ "ls", data_dir },
    });
    defer allocator.free(result.stdout);

    var line_iterator = std.mem.splitScalar(u8, result.stdout, '\n');
    while (line_iterator.next()) |entry_name| {
        if (entry_name.len == 0 or !std.mem.endsWith(u8, entry_name, ".json")) {
            continue;
        }

        const file_path = try std.fs.path.join(allocator, &.{ data_dir, entry_name });
        defer allocator.free(file_path);
        
        std.debug.print("  -> Processing: {s}\n", .{file_path});

        const json_data = try std.fs.cwd().readFileAlloc(allocator, file_path, 100 * 1024 * 1024);
        defer allocator.free(json_data);

        var json_dom = try std.json.parseFromSlice(std.json.Value, allocator, json_data, .{});
        defer json_dom.deinit();

        const root_object = json_dom.value.object;

        const icons_value = root_object.get("icons") orelse {
            std.debug.print("  -> WARNING: Skipping file {s}, no 'icons' field found.\n", .{file_path});
            continue;
        };
        
        const icons_object = icons_value.object;
        const count_in_file = icons_object.count();
        total_icon_count += @intCast(count_in_file);
    }

    std.debug.print("\nTransformer: Found a total of {d} icons across all files.\n", .{total_icon_count});

    const binary_file = try std.fs.cwd().createFile("icons.bin", .{});
    defer binary_file.close();
    const writer = binary_file.writer();

    try writer.writeInt(u32, total_icon_count, .little);

    std.debug.print("Transformer: Successfully created icons.bin.\n", .{});
}
