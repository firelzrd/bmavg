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
#define bmavg_abs(a, b) ((a < b)? b - a : a - b)
#endif // BMAVG_H

static int clz_u8 (uint8_t  v) {return __builtin_clz ((uint32_t)v) - 24;}
static int clz_u16(uint16_t v) {return __builtin_clz ((uint32_t)v) - 16;}
static int clz_u32(uint32_t v) {return __builtin_clz ((uint32_t)v);}
static int clz_u64(uint64_t v) {return __builtin_clzl((uint64_t)v);}

static int fls_u8 (uint8_t  v) {return 32 - __builtin_clz ((uint32_t)v);}
static int fls_u16(uint16_t v) {return 32 - __builtin_clz ((uint32_t)v);}
static int fls_u32(uint32_t v) {return 32 - __builtin_clz ((uint32_t)v);}
static int fls_u64(uint64_t v) {return 64 - __builtin_clzl((uint64_t)v);}

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
	int pos, decay = 0; \
	uint##BITLEN##_t sum = 0, next, diff_abs; \
	if (bmavg_unlikely(!bmavg->count)) return 0; \
	pos = fls_u##BITLEN(bmavg->count) - 1; \
	if (pos > bmavg->limit_bitlen) \
		pos = bmavg->limit_bitlen; \
	for (; pos >= 0; pos--, decay++) { \
		if (bmavg_bit(bmavg->count, pos)) { \
			next = bmavg->hist[pos]; \
			diff_abs = bmavg_abs(sum, next) >> decay; \
			if (diff_abs) \
				sum += (sum < next)? diff_abs : -diff_abs; \
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
