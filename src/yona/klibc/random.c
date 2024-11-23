# include "klibc/types.h"
# include "klibc/print.h"

uint32_t rand() {
    static uint32_t seed = 786521034;

    seed = (1023515347 * seed + 1337) % (1 << 31); // LCG formula
    return seed;
}