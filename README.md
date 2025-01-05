# Binary-Merge Average Filter (bmavg)

- Spacial O(log N)
- Temporal O(log N)

## 64-bit variant:
58 history buckets  
64 - log2(58) >= 58  
Up to 288,230,376,151,711,744 samples  
Up to 288,230,376,151,711,743 as each value  
History size = 464 bytes  
**Yes! Up to approximately 288 quadrillion samples with just 464 bytes of memory.**
## 32-bit variant:
28 history buckets  
32 - log2(27) >= 27  
Up to 134,217,728 samples  
Up to 134,217,727 as each value  
History size = 112 bytes
## 16-bit variant:
12 history buckets  
16 - log2(12) >= 12  
Up to 4,096 samples  
Up to 4,095 as each value  
History size = 24 bytes
## 8-bit variant:
5 history buckets  
8 - log2(5) >= 5  
Up to 32 samples  
Up to 31 as each value  
History size = 5 bytes
