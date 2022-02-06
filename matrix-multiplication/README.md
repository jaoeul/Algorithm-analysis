# Matrix multiplication

Just testing out some optimization techniques.

## Benchmarks on `Intel(R) Core(TM) i5-8350U CPU @ 1.70GHz`
1024 * 1024 matrix multiplication of single-precision floating-point values.

```
+------------------------------------------+
| Algorithm                | Runtime (sec) |
+--------------------------+---------------+
| Naive                    | 5.330076      |
| Blocking                 | 2.506558      |
| Blocking and vectorizing | 0.158514      |
+--------------------------+---------------+
```

## Prerequisites
- Intel avx2 CPU extension
- gcc

## Build
```bash
make
```

## Run
```bash
./mm
```

## TODO
GPU acceleration
