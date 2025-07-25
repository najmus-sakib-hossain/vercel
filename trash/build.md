// Import the standard build system tools from the Zig standard library.
const std = @import("std");

// The `build` function is the main entry point for the Zig build system.
pub fn build(b: *std.Build) void {
    // --- Start Build Timer ---
    // This timer will measure how long it takes for this `build` function to
    // configure all the build steps.
    var timer = std.time.Timer.start() catch |err| {
        std.debug.print("Failed to start build timer: {}\n", .{err});
        return;
    };

    // Get the target architecture and OS for cross-compilation.
    const target = b.standardTargetOptions(.{});

    // Get the optimization mode (Debug, ReleaseSafe, etc.).
    const optimize = b.standardOptimizeOption(.{});

    // --- Create the Executable ---
    // We define an executable artifact.
    const exe = b.addExecutable(.{
        .name = "c_file_creator",
        .target = target,
        .optimize = optimize,
    });

    // --- Add C Source File ---
    // We explicitly add `main.c` as a C source file.
    exe.addCSourceFile(.{
        .file = .{ .path = "main.c" },
        .flags = &.{}, // You could add C flags like "-std=c11" here if needed.
    });

    // --- Link Libraries ---
    // We tell Zig to link against the standard C library.
    exe.linkSystemLibrary("c");

    // --- Install Step ---
    // This tells the build system to install the compiled executable.
    b.installArtifact(exe);

    // --- Create a "Run" Step ---
    // This creates the convenient `zig build run` command.
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_cmd.step);

    // --- Report Build Configuration Time ---
    // We read the timer and calculate the elapsed time in milliseconds.
    const elapsed_ms = @as(f64, @floatFromInt(timer.read())) / 1_000_000.0;
    // `std.debug.print` is the reliable way to print from a build script.
    // This will appear before the output from your C program.
    std.debug.print("Build configuration finished in {d:.4}ms.\n", .{elapsed_ms});
}
