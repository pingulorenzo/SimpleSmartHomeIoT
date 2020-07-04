#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdlib.h>

#define OFFSET_VALUE (1)
#define PROBABILITY_UPDATE (0.2)

#define max(a, b) ((a > b) ? a : b)

#define min(a, b) ((a < b) ? a : b)

#define init_value(value, val_min, val_max)                                             \
        value = ((float)rand() / RAND_MAX) * (val_max - val_min) + val_min

#define update_value(value)                                                             \
        if (((float)rand() / RAND_MAX) < PROBABILITY_UPDATE)                            \
            value += ((float)rand() / RAND_MAX) * (2 * OFFSET_VALUE) - OFFSET_VALUE

#define update_value_inc(value)                                   \
        value += ((float)rand() / RAND_MAX) * (2 * OFFSET_VALUE)

#define update_value_dec(value)                                   \
        value -= ((float)rand() / RAND_MAX) * (2 * OFFSET_VALUE)

#endif /* UTILITIES_H */