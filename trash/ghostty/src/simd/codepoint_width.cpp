// Generates code for every target that this compiler can support.
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "simd/codepoint_width.cpp"  // this file
#include <hwy/foreach_target.h>  // must come before highway.h
#include <hwy/highway.h>
#include <hwy/print-inl.h>

#include <cassert>

HWY_BEFORE_NAMESPACE();
namespace ghostty {
namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

// East Asian Width
HWY_ALIGN constexpr uint32_t eaw_gte32[] = {
    0x16fe0, 0x16ff0, 0x17000, 0x18800, 0x18d00, 0x1aff0, 0x1aff5, 0x1affd,
    0x1b000, 0x1b132, 0x1b150, 0x1b155, 0x1b164, 0x1b170, 0x1f004, 0x1f0cf,
    0x1f18e, 0x1f191, 0x1f200, 0x1f210, 0x1f240, 0x1f250, 0x1f260, 0x1f300,
    0x1f32d, 0x1f337, 0x1f37e, 0x1f3a0, 0x1f3cf, 0x1f3e0, 0x1f3f4, 0x1f3f8,
    0x1f3fb, 0x1f400, 0x1f440, 0x1f442, 0x1f4ff, 0x1f54b, 0x1f550, 0x1f57a,
    0x1f595, 0x1f5a4, 0x1f5fb, 0x1f680, 0x1f6cc, 0x1f6d0, 0x1f6d5, 0x1f6dc,
    0x1f6eb, 0x1f6f4, 0x1f7e0, 0x1f7f0, 0x1f90c, 0x1f93c, 0x1f947, 0x1fa70,
    0x1fa80, 0x1fa90, 0x1fabf, 0x1face, 0x1fae0, 0x1faf0, 0x20000, 0x2a700,
    0x2b740, 0x2b820, 0x2ceb0, 0x2f800, 0x30000, 0x31350, 0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
};

HWY_ALIGN constexpr uint32_t eaw_lte32[] = {
    0x16fe3, 0x16ff1, 0x187f7, 0x18cd5, 0x18d08, 0x1aff3, 0x1affb, 0x1affe,
    0x1b122, 0x1b132, 0x1b152, 0x1b155, 0x1b167, 0x1b2fb, 0x1f004, 0x1f0cf,
    0x1f18e, 0x1f19a, 0x1f202, 0x1f23b, 0x1f248, 0x1f251, 0x1f265, 0x1f320,
    0x1f335, 0x1f37c, 0x1f393, 0x1f3ca, 0x1f3d3, 0x1f3f0, 0x1f3f4, 0x1f3fa,
    0x1f3ff, 0x1f43e, 0x1f440, 0x1f4fc, 0x1f53d, 0x1f54e, 0x1f567, 0x1f57a,
    0x1f596, 0x1f5a4, 0x1f64f, 0x1f6c5, 0x1f6cc, 0x1f6d2, 0x1f6d7, 0x1f6df,
    0x1f6ec, 0x1f6fc, 0x1f7eb, 0x1f7f0, 0x1f93a, 0x1f945, 0x1f9ff, 0x1fa7c,
    0x1fa88, 0x1fabd, 0x1fac5, 0x1fadb, 0x1fae8, 0x1faf8, 0x2a6df, 0x2b739,
    0x2b81d, 0x2cea1, 0x2ebe0, 0x2fa1d, 0x3134a, 0x323af, 0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
};

HWY_ALIGN constexpr uint16_t eaw_gte16[] = {
    0x3000, 0xff01, 0xffe0, 0x1100, 0x231a, 0x2329, 0x232a, 0x23e9, 0x23f0,
    0x23f3, 0x25fd, 0x2614, 0x2648, 0x267f, 0x2693, 0x26a1, 0x26aa, 0x26bd,
    0x26c4, 0x26ce, 0x26d4, 0x26ea, 0x26f2, 0x26f5, 0x26fa, 0x26fd, 0x2705,
    0x270a, 0x2728, 0x274c, 0x274e, 0x2753, 0x2757, 0x2795, 0x27b0, 0x27bf,
    0x2b1b, 0x2b50, 0x2b55, 0x2e80, 0x2e9b, 0x2f00, 0x2ff0, 0x3001, 0x302e,
    0x3041, 0x309b, 0x309d, 0x309f, 0x30a0, 0x30a1, 0x30fb, 0x30fc, 0x30ff,
    0x3105, 0x3131, 0x3190, 0x3192, 0x3196, 0x31a0, 0x31c0, 0x31f0, 0x3200,
    0x3220, 0x322a, 0x3250, 0x3251, 0x3260, 0x3280, 0x328a, 0x32b1, 0x32c0,
    0x3400, 0x4e00, 0xa015, 0xa016, 0xa490, 0xa960, 0xac00, 0xf900, 0xfa70,
    0xfe10, 0xfe30, 0xfe54, 0xfe68, 0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
};

HWY_ALIGN constexpr uint16_t eaw_lte16[] = {
    0x3000, 0xff60, 0xffe6, 0x115f, 0x231b, 0x2329, 0x232a, 0x23ec, 0x23f0,
    0x23f3, 0x25fe, 0x2615, 0x2653, 0x267f, 0x2693, 0x26a1, 0x26ab, 0x26be,
    0x26c5, 0x26ce, 0x26d4, 0x26ea, 0x26f3, 0x26f5, 0x26fa, 0x26fd, 0x2705,
    0x270b, 0x2728, 0x274c, 0x274e, 0x2755, 0x2757, 0x2797, 0x27b0, 0x27bf,
    0x2b1c, 0x2b50, 0x2b55, 0x2e99, 0x2ef3, 0x2fd5, 0x2ffb, 0x3029, 0x303e,
    0x3096, 0x309c, 0x309e, 0x309f, 0x30a0, 0x30fa, 0x30fb, 0x30fe, 0x30ff,
    0x312f, 0x318e, 0x3191, 0x3195, 0x319f, 0x31bf, 0x31e3, 0x31ff, 0x321e,
    0x3229, 0x3247, 0x3250, 0x325f, 0x327f, 0x3289, 0x32b0, 0x32bf, 0x33ff,
    0x4bdf, 0xa014, 0xa015, 0xa48c, 0xa4c6, 0xa97c, 0xd7a3, 0xfa6d, 0xfad9,
    0xfe19, 0xfe52, 0xfe66, 0xfe6b, 0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
};

/// These are the ranges of codepoints that are DEFINITELY width 0.
HWY_ALIGN constexpr uint32_t zero_gte32[] = {
    0x110bd, 0x110cd, 0x13430, 0x1bca0, 0x1d173, 0xe0001, 0xe0020, 0, 0,
    0,       0,       0,       0,       0,       0,       0,       0, 0,
};

HWY_ALIGN constexpr uint32_t zero_lte32[] = {
    0x110bd, 0x110cd, 0x1343f, 0x1bca3, 0x1d17a, 0xe0001, 0xe007f, 0, 0,
    0,       0,       0,       0,       0,       0,       0,       0, 0,
};

HWY_ALIGN constexpr uint16_t zero_gte16[] = {
    0xad,   0x70f, 0x890,  0x180e, 0x200b, 0x202a, 0x2060, 0x2066, 0xfeff,
    0xfff9, 0x488, 0x1abe, 0x20dd, 0x20e2, 0xa670, 0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
};

HWY_ALIGN constexpr uint16_t zero_lte16[] = {
    0xad,   0x70f, 0x891,  0x180e, 0x200f, 0x202e, 0x2064, 0x206f, 0xfeff,
    0xfffb, 0x489, 0x1abe, 0x20e0, 0x20e4, 0xa672, 0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
    0,      0,     0,      0,      0,      0,      0,      0,      0,
};

/// Non-spacing marks
HWY_ALIGN constexpr uint32_t nsm_gte32[] = {
    0x101fd, 0x102e0, 0x10376, 0x10a01, 0x10a05, 0x10a0c, 0x10a38, 0x10a3f,
    0x10ae5, 0x10d24, 0x10eab, 0x10efd, 0x10f46, 0x10f82, 0x11001, 0x11038,
    0x11070, 0x11073, 0x1107f, 0x110b3, 0x110b9, 0x110c2, 0x11100, 0x11127,
    0x1112d, 0x11173, 0x11180, 0x111b6, 0x111c9, 0x111cf, 0x1122f, 0x11234,
    0x11236, 0x1123e, 0x11241, 0x112df, 0x112e3, 0x11300, 0x1133b, 0x11340,
    0x11366, 0x11370, 0x11438, 0x11442, 0x11446, 0x1145e, 0x114b3, 0x114ba,
    0x114bf, 0x114c2, 0x115b2, 0x115bc, 0x115bf, 0x115dc, 0x11633, 0x1163d,
    0x1163f, 0x116ab, 0x116ad, 0x116b0, 0x116b7, 0x1171d, 0x11722, 0x11727,
    0x1182f, 0x11839, 0x1193b, 0x1193e, 0x11943, 0x119d4, 0x119da, 0x119e0,
    0x11a01, 0x11a33, 0x11a3b, 0x11a47, 0x11a51, 0x11a59, 0x11a8a, 0x11a98,
    0x11c30, 0x11c38, 0x11c3f, 0x11c92, 0x11caa, 0x11cb2, 0x11cb5, 0x11d31,
    0x11d3a, 0x11d3c, 0x11d3f, 0x11d47, 0x11d90, 0x11d95, 0x11d97, 0x11ef3,
    0x11f00, 0x11f36, 0x11f40, 0x11f42, 0x13440, 0x13447, 0x16af0, 0x16b30,
    0x16f4f, 0x16f8f, 0x16fe4, 0x1bc9d, 0x1cf00, 0x1cf30, 0x1d167, 0x1d17b,
    0x1d185, 0x1d1aa, 0x1d242, 0x1da00, 0x1da3b, 0x1da75, 0x1da84, 0x1da9b,
    0x1daa1, 0x1e000, 0x1e008, 0x1e01b, 0x1e023, 0x1e026, 0x1e08f, 0x1e130,
    0x1e2ae, 0x1e2ec, 0x1e4ec, 0x1e8d0, 0x1e944, 0xe0100, 0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
};

HWY_ALIGN constexpr uint32_t nsm_lte32[] = {
    0x101fd, 0x102e0, 0x1037a, 0x10a03, 0x10a06, 0x10a0f, 0x10a3a, 0x10a3f,
    0x10ae6, 0x10d27, 0x10eac, 0x10eff, 0x10f50, 0x10f85, 0x11001, 0x11046,
    0x11070, 0x11074, 0x11081, 0x110b6, 0x110ba, 0x110c2, 0x11102, 0x1112b,
    0x11134, 0x11173, 0x11181, 0x111be, 0x111cc, 0x111cf, 0x11231, 0x11234,
    0x11237, 0x1123e, 0x11241, 0x112df, 0x112ea, 0x11301, 0x1133c, 0x11340,
    0x1136c, 0x11374, 0x1143f, 0x11444, 0x11446, 0x1145e, 0x114b8, 0x114ba,
    0x114c0, 0x114c3, 0x115b5, 0x115bd, 0x115c0, 0x115dd, 0x1163a, 0x1163d,
    0x11640, 0x116ab, 0x116ad, 0x116b5, 0x116b7, 0x1171f, 0x11725, 0x1172b,
    0x11837, 0x1183a, 0x1193c, 0x1193e, 0x11943, 0x119d7, 0x119db, 0x119e0,
    0x11a0a, 0x11a38, 0x11a3e, 0x11a47, 0x11a56, 0x11a5b, 0x11a96, 0x11a99,
    0x11c36, 0x11c3d, 0x11c3f, 0x11ca7, 0x11cb0, 0x11cb3, 0x11cb6, 0x11d36,
    0x11d3a, 0x11d3d, 0x11d45, 0x11d47, 0x11d91, 0x11d95, 0x11d97, 0x11ef4,
    0x11f01, 0x11f3a, 0x11f40, 0x11f42, 0x13440, 0x13455, 0x16af4, 0x16b36,
    0x16f4f, 0x16f92, 0x16fe4, 0x1bc9e, 0x1cf2d, 0x1cf46, 0x1d169, 0x1d182,
    0x1d18b, 0x1d1ad, 0x1d244, 0x1da36, 0x1da6c, 0x1da75, 0x1da84, 0x1da9f,
    0x1daaf, 0x1e006, 0x1e018, 0x1e021, 0x1e024, 0x1e02a, 0x1e08f, 0x1e136,
    0x1e2ae, 0x1e2ef, 0x1e4ef, 0x1e8d6, 0x1e94a, 0xe01ef, 0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
    0,       0,       0,       0,       0,       0,       0,       0,
};

HWY_ALIGN constexpr uint16_t nsm_gte16[] = {
    0x300,  0x483,  0x591,  0x5bf,  0x5c1,  0x5c4,  0x5c7,  0x610,  0x64b,
    0x670,  0x6d6,  0x6df,  0x6e7,  0x6ea,  0x711,  0x730,  0x7a6,  0x7eb,
    0x7fd,  0x816,  0x81b,  0x825,  0x829,  0x859,  0x898,  0x8ca,  0x8e3,
    0x93a,  0x93c,  0x941,  0x94d,  0x951,  0x962,  0x981,  0x9bc,  0x9c1,
    0x9cd,  0x9e2,  0x9fe,  0xa01,  0xa3c,  0xa41,  0xa47,  0xa4b,  0xa51,
    0xa70,  0xa75,  0xa81,  0xabc,  0xac1,  0xac7,  0xacd,  0xae2,  0xafa,
    0xb01,  0xb3c,  0xb3f,  0xb41,  0xb4d,  0xb55,  0xb62,  0xb82,  0xbc0,
    0xbcd,  0xc00,  0xc04,  0xc3c,  0xc3e,  0xc46,  0xc4a,  0xc55,  0xc62,
    0xc81,  0xcbc,  0xcbf,  0xcc6,  0xccc,  0xce2,  0xd00,  0xd3b,  0xd41,
    0xd4d,  0xd62,  0xd81,  0xdca,  0xdd2,  0xdd6,  0xe31,  0xe34,  0xe47,
    0xeb1,  0xeb4,  0xec8,  0xf18,  0xf35,  0xf37,  0xf39,  0xf71,  0xf80,
    0xf86,  0xf8d,  0xf99,  0xfc6,  0x102d, 0x1032, 0x1039, 0x103d, 0x1058,
    0x105e, 0x1071, 0x1082, 0x1085, 0x108d, 0x109d, 0x135d, 0x1712, 0x1732,
    0x1752, 0x1772, 0x17b4, 0x17b7, 0x17c6, 0x17c9, 0x17dd, 0x180b, 0x180f,
    0x1885, 0x18a9, 0x1920, 0x1927, 0x1932, 0x1939, 0x1a17, 0x1a1b, 0x1a56,
    0x1a58, 0x1a60, 0x1a62, 0x1a65, 0x1a73, 0x1a7f, 0x1ab0, 0x1abf, 0x1b00,
    0x1b34, 0x1b36, 0x1b3c, 0x1b42, 0x1b6b, 0x1b80, 0x1ba2, 0x1ba8, 0x1bab,
    0x1be6, 0x1be8, 0x1bed, 0x1bef, 0x1c2c, 0x1c36, 0x1cd0, 0x1cd4, 0x1ce2,
    0x1ced, 0x1cf4, 0x1cf8, 0x1dc0, 0x20d0, 0x20e1, 0x20e5, 0x2cef, 0x2d7f,
    0x2de0, 0x302a, 0x3099, 0xa66f, 0xa674, 0xa69e, 0xa6f0, 0xa802, 0xa806,
    0xa80b, 0xa825, 0xa82c, 0xa8c4, 0xa8e0, 0xa8ff, 0xa926, 0xa947, 0xa980,
    0xa9b3, 0xa9b6, 0xa9bc, 0xa9e5, 0xaa29, 0xaa31, 0xaa35, 0xaa43, 0xaa4c,
    0xaa7c, 0xaab0, 0xaab2, 0xaab7, 0xaabe, 0xaac1, 0xaaec, 0xaaf6, 0xabe5,
    0xabe8, 0xabed, 0xfb1e, 0xfe00, 0xfe20, 0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
};

HWY_ALIGN constexpr uint16_t nsm_lte16[] = {
    0x36f,  0x487,  0x5bd,  0x5bf,  0x5c2,  0x5c5,  0x5c7,  0x61a,  0x65f,
    0x670,  0x6dc,  0x6e4,  0x6e8,  0x6ed,  0x711,  0x74a,  0x7b0,  0x7f3,
    0x7fd,  0x819,  0x823,  0x827,  0x82d,  0x85b,  0x89f,  0x8e1,  0x902,
    0x93a,  0x93c,  0x948,  0x94d,  0x957,  0x963,  0x981,  0x9bc,  0x9c4,
    0x9cd,  0x9e3,  0x9fe,  0xa02,  0xa3c,  0xa42,  0xa48,  0xa4d,  0xa51,
    0xa71,  0xa75,  0xa82,  0xabc,  0xac5,  0xac8,  0xacd,  0xae3,  0xaff,
    0xb01,  0xb3c,  0xb3f,  0xb44,  0xb4d,  0xb56,  0xb63,  0xb82,  0xbc0,
    0xbcd,  0xc00,  0xc04,  0xc3c,  0xc40,  0xc48,  0xc4d,  0xc56,  0xc63,
    0xc81,  0xcbc,  0xcbf,  0xcc6,  0xccd,  0xce3,  0xd01,  0xd3c,  0xd44,
    0xd4d,  0xd63,  0xd81,  0xdca,  0xdd4,  0xdd6,  0xe31,  0xe3a,  0xe4e,
    0xeb1,  0xebc,  0xece,  0xf19,  0xf35,  0xf37,  0xf39,  0xf7e,  0xf84,
    0xf87,  0xf97,  0xfbc,  0xfc6,  0x1030, 0x1037, 0x103a, 0x103e, 0x1059,
    0x1060, 0x1074, 0x1082, 0x1086, 0x108d, 0x109d, 0x135f, 0x1714, 0x1733,
    0x1753, 0x1773, 0x17b5, 0x17bd, 0x17c6, 0x17d3, 0x17dd, 0x180d, 0x180f,
    0x1886, 0x18a9, 0x1922, 0x1928, 0x1932, 0x193b, 0x1a18, 0x1a1b, 0x1a56,
    0x1a5e, 0x1a60, 0x1a62, 0x1a6c, 0x1a7c, 0x1a7f, 0x1abd, 0x1ace, 0x1b03,
    0x1b34, 0x1b3a, 0x1b3c, 0x1b42, 0x1b73, 0x1b81, 0x1ba5, 0x1ba9, 0x1bad,
    0x1be6, 0x1be9, 0x1bed, 0x1bf1, 0x1c33, 0x1c37, 0x1cd2, 0x1ce0, 0x1ce8,
    0x1ced, 0x1cf4, 0x1cf9, 0x1dff, 0x20dc, 0x20e1, 0x20f0, 0x2cf1, 0x2d7f,
    0x2dff, 0x302d, 0x309a, 0xa66f, 0xa67d, 0xa69f, 0xa6f1, 0xa802, 0xa806,
    0xa80b, 0xa826, 0xa82c, 0xa8c5, 0xa8f1, 0xa8ff, 0xa92d, 0xa951, 0xa982,
    0xa9b3, 0xa9b9, 0xa9bd, 0xa9e5, 0xaa2e, 0xaa32, 0xaa36, 0xaa43, 0xaa4c,
    0xaa7c, 0xaab0, 0xaab4, 0xaab8, 0xaabf, 0xaac1, 0xaaed, 0xaaf6, 0xabe5,
    0xabe8, 0xabed, 0xfb1e, 0xfe0f, 0xfe2f, 0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
    0,      0,      0,      0,      0,      0,      0,      0,      0,
};

// All our tables must be identically sized
static_assert(std::size(eaw_gte32) == std::size(eaw_lte32));
static_assert(std::size(eaw_gte16) == std::size(eaw_lte16));
static_assert(std::size(zero_gte32) == std::size(zero_lte32));
static_assert(std::size(zero_gte16) == std::size(zero_lte16));
static_assert(std::size(nsm_gte32) == std::size(nsm_lte32));
static_assert(std::size(nsm_gte16) == std::size(nsm_lte16));

/// Handles 16-bit codepoints.
template <class D, typename T = uint16_t>
int8_t CodepointWidth16(D d, uint16_t input) {
  assert(input > 0xFF);
  assert(input <= 0xFFFF);

  const size_t N = hn::Lanes(d);
  const hn::Vec<D> input_vec = Set(d, input);

  {
    // NOTE: 0x2E3B is technically width 3 but for our terminal we only
    // handle up to width 2 as wide so we will treat it as width 2.
    HWY_ALIGN constexpr T gte_keys[] = {
        0x2E3A, 0x3400, 0x4E00, 0xF900, 0x2E3B, 0x1160, 0x2060, 0xFFF0, 0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    };
    HWY_ALIGN constexpr T lte_keys[] = {
        0x2E3A, 0x4DBF, 0x9FFF, 0xFAFF, 0x2E3B, 0x11FF, 0x206F, 0xFFF8, 0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
        0,      0,      0,      0,      0,      0,      0,      0,      0, 0,
    };
    static_assert(std::size(gte_keys) == std::size(lte_keys));
    static_assert(std::size(gte_keys) >= 32);
    size_t i = 0;
    for (; i + N <= std::size(lte_keys) && lte_keys[i] != 0; i += N) {
      const hn::Vec<D> lte_vec = hn::Load(d, lte_keys + i);
      const hn::Vec<D> gte_vec = hn::Load(d, gte_keys + i);
      const intptr_t idx = hn::FindFirstTrue(
          d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));

      // We organize the data above to split 0 and 2-width codepoints since
      // we can probably do all the comparisons in one go.
      if (idx >= 5) {
        return 0;
      } else if (idx >= 0) {
        return 2;
      }
    }
    assert(i >= 7);  // We should have checked all the ranges.
  }

  {
    constexpr T zero_gte_min =
        *std::min_element(zero_gte16, zero_gte16 + std::size(zero_gte16));
    constexpr T zero_lte_max =
        *std::max_element(zero_lte16, zero_lte16 + std::size(zero_lte16));
    if (input >= zero_gte_min && input <= zero_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(zero_gte16) && zero_gte16[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, zero_lte16 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, zero_gte16 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 0;
        }
      }
    }
  }

  {
    constexpr T eaw_gte_min =
        *std::min_element(eaw_gte16, eaw_gte16 + std::size(eaw_gte16));
    constexpr T eaw_lte_max =
        *std::max_element(eaw_lte16, eaw_lte16 + std::size(eaw_lte16));
    if (input >= eaw_gte_min && input <= eaw_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(eaw_lte16) && eaw_lte16[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, eaw_lte16 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, eaw_gte16 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 2;
        }
      }
    }
  }

  {
    constexpr T nsm_gte_min =
        *std::min_element(nsm_gte16, nsm_gte16 + std::size(nsm_gte16));
    constexpr T nsm_lte_max =
        *std::max_element(nsm_lte16, nsm_lte16 + std::size(nsm_lte16));
    if (input >= nsm_gte_min && input <= nsm_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(nsm_lte16) && nsm_lte16[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, nsm_lte16 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, nsm_gte16 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 0;
        }
      }
    }
  }

  return 1;
}

/// Handles codepoints larger than 16-bit.
template <class D, typename T = uint32_t>
int8_t CodepointWidth32(D d, T input) {
  assert(input > 0xFFFF);

  const size_t N = hn::Lanes(d);
  const hn::Vec<D> input_vec = Set(d, input);

  {
    // NOTE: 0x2E3B is technically width 3 but for our terminal we only
    // handle up to width 2 as wide so we will treat it as width 2.
    HWY_ALIGN constexpr T gte_keys[] = {
        0x1f1e6, 0x20000, 0x30000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    HWY_ALIGN constexpr T lte_keys[] = {
        0x1f1ff, 0x2FFFD, 0x3FFFD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    static_assert(std::size(gte_keys) == std::size(lte_keys));
    static_assert(std::size(gte_keys) >= 16);
    size_t i = 0;
    for (; i + N <= std::size(lte_keys) && lte_keys[i] != 0; i += N) {
      const hn::Vec<D> lte_vec = hn::Load(d, lte_keys + i);
      const hn::Vec<D> gte_vec = hn::Load(d, gte_keys + i);
      const intptr_t idx = hn::FindFirstTrue(
          d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
      if (idx >= 0) {
        return 2;
      }
    }
    assert(i >= 2);  // We should have checked all the ranges.
  }

  {
    HWY_ALIGN constexpr T gte_keys[] = {
        0xE0000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    HWY_ALIGN constexpr T lte_keys[] = {
        0xE0FFF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };
    static_assert(std::size(gte_keys) == std::size(lte_keys));
    static_assert(std::size(gte_keys) >= 16);
    size_t i = 0;
    for (; i + N <= std::size(lte_keys) && lte_keys[i] != 0; i += N) {
      const hn::Vec<D> lte_vec = hn::Load(d, lte_keys + i);
      const hn::Vec<D> gte_vec = hn::Load(d, gte_keys + i);
      const intptr_t idx = hn::FindFirstTrue(
          d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
      if (idx >= 0) {
        return 0;
      }
    }
  }

  {
    constexpr T zero_gte_min =
        *std::min_element(zero_gte32, zero_gte32 + std::size(zero_gte32));
    constexpr T zero_lte_max =
        *std::max_element(zero_lte32, zero_lte32 + std::size(zero_lte32));
    if (input >= zero_gte_min && input <= zero_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(zero_gte32) && zero_gte32[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, zero_lte32 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, zero_gte32 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 0;
        }
      }
    }
  }

  {
    constexpr T eaw_gte_min =
        *std::min_element(eaw_gte32, eaw_gte32 + std::size(eaw_gte32));
    constexpr T eaw_lte_max =
        *std::max_element(eaw_lte32, eaw_lte32 + std::size(eaw_lte32));
    if (input >= eaw_gte_min && input <= eaw_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(eaw_lte32) && eaw_lte32[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, eaw_lte32 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, eaw_gte32 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 2;
        }
      }
    }
  }

  {
    constexpr T nsm_gte_min =
        *std::min_element(nsm_gte32, nsm_gte32 + std::size(nsm_gte32));
    constexpr T nsm_lte_max =
        *std::max_element(nsm_lte32, nsm_lte32 + std::size(nsm_lte32));
    if (input >= nsm_gte_min && input <= nsm_lte_max) {
      size_t i = 0;
      for (; i + N <= std::size(nsm_lte32) && nsm_lte32[i] != 0; i += N) {
        const hn::Vec<D> lte_vec = hn::Load(d, nsm_lte32 + i);
        const hn::Vec<D> gte_vec = hn::Load(d, nsm_gte32 + i);
        const intptr_t idx = hn::FindFirstTrue(
            d, hn::And(hn::Le(input_vec, lte_vec), hn::Ge(input_vec, gte_vec)));
        if (idx >= 0) {
          return 0;
        }
      }
    }
  }

  return 1;
}

/// Vectorized implementation of Unicode display width. Determining width
/// unfortunately requires many small range checks, so we test some fast paths
/// and otherwise try to do N (vector lane width) range checks at a time.
int8_t CodepointWidth(uint32_t input) {
  // If the input is ASCII, then we return 1. We do NOT check for
  // control characters because we assume that the input has already
  // been checked for that case.
  if (input <= 0xFF) {
    return 1;
  }

  // We handle 16-bit codepoints separately because they are more common
  // and we can fit double the number of lanes in a vector.
  if (input <= 0xFFFF) {
    hn::ScalableTag<uint16_t> d;
    return CodepointWidth16(d, input);
  }

  hn::ScalableTag<uint32_t> d;
  return CodepointWidth32(d, input);
}

}  // namespace HWY_NAMESPACE
}  // namespace ghostty
HWY_AFTER_NAMESPACE();

// HWY_ONCE is true for only one of the target passes
#if HWY_ONCE

namespace ghostty {

HWY_EXPORT(CodepointWidth);

int8_t CodepointWidth(uint32_t cp) {
  return HWY_DYNAMIC_DISPATCH(CodepointWidth)(cp);
}

}  // namespace ghostty

extern "C" {

int8_t ghostty_simd_codepoint_width(uint32_t cp) {
  return ghostty::CodepointWidth(cp);
}

}  // extern "C"

#endif  // HWY_ONCE
