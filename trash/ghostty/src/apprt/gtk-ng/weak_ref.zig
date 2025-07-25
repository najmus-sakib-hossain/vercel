const std = @import("std");
const gtk = @import("gtk");
const gobject = @import("gobject");

/// A lightweight wrapper around gobject.WeakRef to make it type-safe
/// to hold a single type of value.
pub fn WeakRef(comptime T: type) type {
    return struct {
        const Self = @This();

        ref: gobject.WeakRef = std.mem.zeroes(gobject.WeakRef),

        /// Set the weak reference to the given object. This will not
        /// increase the reference count of the object.
        pub fn set(self: *Self, v_: ?*T) void {
            if (v_) |v| {
                self.ref.set(v.as(gobject.Object));
            } else {
                self.ref.set(null);
            }
        }

        /// Get a strong reference to the object, or null if the object
        /// has been finalized. This increases the reference count by one.
        pub fn get(self: *Self) ?*T {
            // The GIR of g_weak_ref_get has a bug where the optional
            // is not encoded. Or, it may be a bug in zig-gobject.
            const obj_: ?*gobject.Object = @ptrCast(self.ref.get());
            const obj = obj_ orelse return null;

            // We can't use `as` because `as` guarantees conversion and
            // that can't be statically guaranteed.
            return gobject.ext.cast(T, obj);
        }
    };
}

test WeakRef {
    const testing = std.testing;

    var ref: WeakRef(gtk.TextBuffer) = .{};
    const obj: *gtk.TextBuffer = .new(null);
    ref.set(obj);
    ref.get().?.unref(); // The "?" asserts non-null
    obj.unref();
    try testing.expect(ref.get() == null);
}
