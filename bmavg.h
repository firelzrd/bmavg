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
#define bmavg_shr(v, len) ((0 < len) ? v >> len : v << -len)
#define bmavg_abs(a, b) ((a < b)? b - a : a - b)

#define DEFINE_BMAVG_DECL(BITLEN, HIST_COUNT) \
const int bmavg_hist_count_u##BITLEN = HIST_COUNT; \
struct bmavg_u##BITLEN { \
    uint##BITLEN##_t hist[HIST_COUNT]; \
    uint##BITLEN##_t count; \
	int limit_bitlen; \
}; \
void bmavg_write_u##BITLEN(struct bmavg_u##BITLEN *bmavg, uint##BITLEN##_t v); \
uint##BITLEN##_t bmavg_read_u##BITLEN(struct bmavg_u##BITLEN *bmavg); \
void bmavg_init_u##BITLEN(struct bmavg_u##BITLEN *bmavg); \
void bmavg_set_limit_u##BITLEN(struct bmavg_u##BITLEN *bmavg, int limit);

DEFINE_BMAVG_DECL(8, 5)
DEFINE_BMAVG_DECL(16, 12)
DEFINE_BMAVG_DECL(32, 28)
DEFINE_BMAVG_DECL(64, 58)

#endif // BMAVG_H
