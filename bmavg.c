/*
 *  Binary-Merge Average Filter (bmavg)
 *  (C) 2025 Masahito Suzuki
 */
#include <stdint.h>

#ifndef BMAVG_H
#define bmavg_swap(a, b) \
	({ \
	__typeof__(a) temp = a; \
	a = b; \
	temp; \
	})

#define bmavg_unlikely(x) __builtin_expect(!!(x), 0)
#define bmavg_bit(v, p)  (((__typeof__(v))(v) >> (p)) & 1)
#define bmavg_mask(p) ((__typeof__(v))1 << (p))
#endif // BMAVG_H

#define DEFINE_BMAVG(BITLEN, HIST_COUNT) \
struct bmavg_u##BITLEN { \
	uint##BITLEN##_t hist[HIST_COUNT]; \
	uint##BITLEN##_t count; \
	int limit_bitlen; \
}; \
void bmavg_write_u##BITLEN(struct bmavg_u##BITLEN *bmavg, uint##BITLEN##_t v) { \
	uint##BITLEN##_t curr = bmavg->count, \
		fmask = bmavg_mask(bmavg->limit_bitlen), \
		hmask = bmavg_mask(bmavg->limit_bitlen - 1), \
		full = (curr == fmask - 1); \
	if (bmavg_unlikely(full)) \
		curr = hmask - 1; \
	uint##BITLEN##_t next = curr + 1, \
		should_halve = !(next % hmask), \
		carried = v, pos = 0, \
		carry_bmp = (next ^ curr) >> 1; \
	for (; bmavg_bit(carry_bmp, pos); pos++) \
		carried = (carried + bmavg_swap(bmavg->hist[pos], 0) + 1) / 2; \
	if (bmavg_unlikely(full)) \
		carried = (carried + bmavg->hist[pos] + 1) / 2; \
	bmavg->hist[pos] = carried; \
	bmavg->count++; \
	if (bmavg_unlikely(should_halve)) \
		bmavg->count = hmask; \
	return; \
} \
uint##BITLEN##_t bmavg_read_u##BITLEN(struct bmavg_u##BITLEN *bmavg) { \
	uint##BITLEN##_t sum = 0, pos = 0, hweight, lweight, delta; \
	if (bmavg_unlikely(!bmavg->count)) \
		return 0; \
	for (; pos < bmavg->limit_bitlen; pos++) { \
		if (bmavg_bit(bmavg->count, pos)) { \
			sum = bmavg->hist[pos]; \
			break; \
		} \
	} \
	for (; ++pos < bmavg->limit_bitlen;) { \
		if (bmavg_bit(bmavg->count, pos)) { \
			hweight = 1 << pos; \
			lweight = bmavg->count & ((1 << pos) - 1); \
			sum = (sum < bmavg->hist[pos]) ? \
				sum + ((bmavg->hist[pos] - sum) * hweight) / (hweight + lweight): \
				sum - ((sum - bmavg->hist[pos]) * hweight) / (hweight + lweight); \
		} \
	} \
	return sum; \
} \
void bmavg_init_u##BITLEN(struct bmavg_u##BITLEN *bmavg) { \
	for (int pos = 0; pos < HIST_COUNT; pos++) \
		bmavg->hist[pos] = 0; \
	bmavg->limit_bitlen = HIST_COUNT; \
	bmavg->count = 0; \
} \
void bmavg_set_limit_u##BITLEN(struct bmavg_u##BITLEN *bmavg, int limit) { \
	for (int pos = limit; pos < HIST_COUNT; pos++) \
		bmavg->hist[pos] = 0; \
	bmavg->limit_bitlen = limit; \
	bmavg->count &= (1 << limit) - 1; \
} 

DEFINE_BMAVG( 8,  5)
DEFINE_BMAVG(16, 12)
DEFINE_BMAVG(32, 28)
DEFINE_BMAVG(64, 58)
