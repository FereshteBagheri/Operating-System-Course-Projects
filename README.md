# Operating Systems Course Projects

Two C++ projects for the Operating Systems course at the University of Tehran: one about processes and inter-process communication, and one about multithreading. Each folder has the assignment description (`Description.pdf`, in Persian).

## 1. Map Reduce Utility Bill Calculator — processes and pipes

Calculates water, gas and electricity statistics and bills for a set of buildings from hourly usage data (`code/buildings/<building>/<resource>.csv`). The work is split across processes in a map-reduce style:

- **`main`** finds the buildings, then uses `fork()` and `exec()` to start one **building process** per building and one **bill process**.
- Each **building process** starts one **resource process** per resource file (water, gas, electricity) and reads its results through an **unnamed pipe**.
- Each **resource process** reads its CSV file and computes, for each month, the total usage, the average usage, the peak hour and the usage in the peak hour.
- The building processes send their results to the **bill process** through **named pipes** (FIFOs). The bill process computes the bills and answers questions from the user: for a building, resource and month, it can show the `bill`, `total-usage`, `average`, `peak-hour` and `peak-hour-usage`.
- Every process writes coloured log messages, so the order of events is easy to follow.

```bash
cd "Map Reduce Utility Bill Calculator/code"
make
./main
```

## 2. Parallel Image Processing — threads

Applies a chain of filters to a 24-bit BMP image: read → vertical flip → blur (3×3 kernel) → "purple haze" colour filter → diagonal lines → write. There are two versions:

- **`serial/`** runs everything in one thread.
- **`parallel/`** splits the image into horizontal parts and uses **POSIX threads** (`pthread_create` / `pthread_join`, 4 threads by default) for reading and writing the file and for the blur and purple-haze filters.

Both versions write the result to `output.bmp` and print the time each step takes, so the speed-up of the parallel version can be measured.

```bash
cd "Parallel Image Processing/parallel"   # or serial
make
./ImageFilters input.bmp
```
