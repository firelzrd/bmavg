#ifndef BMAVG_H
#define BMAVG_H

#include <stdint.h>

#define bmavg_swap(a, b) \
	({ \
	__typeof__(a) temp = a; \
	a = b; \
	temp; \
	})

#define bmavg_unlikely(x) __builtin_expect(!!(x), 0)
#define bmavg_bit(v, p)  (((__typeof__(v))(v) >> (p)) & 1)
#define bmavg_mask(p) ((__typeof__(v))1 << (p))
#define bmavg_abs(a, b) ((a < b)? b - a : a - b)


typedef uint8_t   bmavg_uint8_t;
typedef uint16_t  bmavg_uint16_t;
typedef uint32_t  bmavg_uint32_t;
typedef uint64_t  bmavg_uint64_t;
typedef __uint128_t bmavg_uint128_t;

#define BMAVG_CONCAT_RAW(a, b) a##b
#define BMAVG_CONCAT(a, b) BMAVG_CONCAT_RAW(a, b)
#define bmavg_uint(N) BMAVG_CONCAT(BMAVG_CONCAT(bmavg_uint, N), _t)

#define DEFINE_BMAVG_DECL(BITLEN, HIST_COUNT) \
extern const int bmavg_hist_count_u##BITLEN; \
struct bmavg_u##BITLEN { \
    bmavg_uint(BITLEN) hist[HIST_COUNT]; \
    bmavg_uint(BITLEN) count; \
	int limit_bitlen; \
}; \
void bmavg_write_u##BITLEN(struct bmavg_u##BITLEN *bmavg, bmavg_uint(BITLEN) v); \
bmavg_uint(BITLEN) bmavg_read_u##BITLEN(struct bmavg_u##BITLEN *bmavg); \
void bmavg_init_u##BITLEN(struct bmavg_u##BITLEN *bmavg); \
void bmavg_set_limit_u##BITLEN(struct bmavg_u##BITLEN *bmavg, int limit);

DEFINE_BMAVG_DECL(8, 5)
DEFINE_BMAVG_DECL(16, 12)
DEFINE_BMAVG_DECL(32, 28)
DEFINE_BMAVG_DECL(64, 58)

#endif // BMAVG_H
