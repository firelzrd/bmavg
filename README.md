# Binary-Merge Average Filter (bmavg)

A space-efficient and high-performance moving average filter library for C.

## Features

- **Space Complexity**: O(log N) - Drastically reduces memory usage compared to traditional moving averages that store all samples.
- **Time Complexity**: **Amortized O(1)** (Worst-case O(log N)) - Extremely fast processing for almost every sample.
- **Accuracy**: Calculates the exact average of the window, not an approximation.
- **Type Support**: Optimized variants for 8, 16, 32, and 64-bit unsigned integers.

### Variants and Capacities

| Variant | History Buckets | Max Window Size (Samples) | Max Value Per Sample | History Size |
| :--- | :--- | :--- | :--- | :--- |
| **64-bit** | 58 | 288,230,376,151,711,744 | 288,230,376,151,711,743 | 464 bytes |
| **32-bit** | 28 | 134,217,728 | 134,217,727 | 112 bytes |
| **16-bit** | 12 | 4,096 | 4,095 | 24 bytes |
| **8-bit** | 5 | 32 | 31 | 5 bytes |

The 64-bit variant can track up to approximately **288 quadrillion samples** using just **464 bytes** of memory.

## Ideal Use Cases

This library demonstrates its true power in scenarios requiring **exact averages** over **massive datasets** within **strictly limited memory**:

1.  **High-Volume Streaming**
    *   **Traditional Moving Average**: Requires O(N) memory. Storing millions of samples consumes megabytes or gigabytes.
    *   **bmavg**: Requires **O(log N)** memory. Computing the exact average of **quadrillions of samples** takes less than **500 bytes** of RAM.

2.  **Embedded Systems & IoT**
    *   Perfect for microcontrollers and edge devices with extreme resource constraints. It enables high-precision, long-term statistical tracking without allocating large buffers.

3.  **Exactness over Approximation**
    *   **EMA (Exponential Moving Average)**: Memory-efficient but provides an approximation that biases towards recent values.
    *   **bmavg**: Calculates the **mathematically exact average** of the sliding window, treating all samples in the window equally.

## Usage

### Integration

1.  Include `bmavg.h` in your C source file.
2.  Compile your code with `bmavg.c`.

```c
#include "bmavg.h"
```

### API Reference

Replace `uXX` with `u8`, `u16`, `u32`, or `u64`.

*   **`void bmavg_init_uXX(struct bmavg_uXX *bmavg)`**
    Initializes the filter structure.

*   **`void bmavg_write_uXX(struct bmavg_uXX *bmavg, bmavg_uint(XX) v)`**
    Adds a new value `v` to the filter.

*   **`bmavg_uint(XX) bmavg_read_uXX(struct bmavg_uXX *bmavg)`**
    Returns the current average.

*   **`void bmavg_set_limit_uXX(struct bmavg_uXX *bmavg, int limit)`**
    (Optional) Sets a custom limit on the history size (power of 2).

### Example

```c
#include <stdio.h>
#include "bmavg.h"

int main(void) {
    // 1. Declare the filter structure
    struct bmavg_u32 avg_filter;

    // 2. Initialize
    bmavg_init_u32(&avg_filter);

    // 3. Add data
    bmavg_write_u32(&avg_filter, 100);
    bmavg_write_u32(&avg_filter, 200);
    bmavg_write_u32(&avg_filter, 300);

    // 4. Read average
    // Should be (100+200+300)/3 = 200
    printf("Average: %u\n", bmavg_read_u32(&avg_filter)); 

    return 0;
}
```

## Build and Test

To verify the library, you can compile and run the included sample program:

```bash
gcc -o bmavg_sample bmavg_sample.c bmavg.c
./bmavg_sample
```

