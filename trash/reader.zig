const std = @import("std");

pub fn main() !void {
    const binary_file = try std.fs.cwd().openFile("icons.bin", .{});
    defer binary_file.close();
    const reader = binary_file.reader();
    const icon_count = try reader.readInt(u32, .little);
    std.debug.print("Reader: The binary data contains {d} icons.\n", .{icon_count});
}
