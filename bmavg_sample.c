#include <stdio.h>
#include "bmavg.h"

#define SAMPLE_COUNT 50
int sample_data[50] = {10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                       30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                       50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
                       70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
                       90, 90, 90, 90, 90, 90, 90, 90, 90, 90};

#define DEFINE_TEST_DECL(BITLEN) \
void show_hist_u##BITLEN(struct bmavg_u##BITLEN *bmavg) { \
	for (int i = 0; i < bmavg_hist_count_u##BITLEN; i++) \
		printf("%u ", bmavg->hist[i]); \
	printf("\n"); \
} \
void test_u##BITLEN() { \
	struct bmavg_u##BITLEN bmavg; \
	bmavg_init_u##BITLEN(&bmavg); \
	/*bmavg_set_limit_u##BITLEN(&bmavg, 4);*/ \
	for (int i = 0; i < SAMPLE_COUNT; i++) { \
		printf("Loop %u:\n", i + 1); \
		bmavg_write_u##BITLEN(&bmavg, sample_data[i]); \
		printf("  added %d, avg: %d\n", sample_data[i], bmavg_read_u##BITLEN(&bmavg)); \
		printf("  "); \
		show_hist_u##BITLEN(&bmavg); \
	} \
}

DEFINE_TEST_DECL( 8)
DEFINE_TEST_DECL(16)
DEFINE_TEST_DECL(32)

int main(void) {
	test_u8();
	test_u16();
	test_u32();
	return 0;
}
