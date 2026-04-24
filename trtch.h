#ifndef _TRTCH_H
#define _TRTCH_H

#include <stddef.h>

typedef enum _PARITY {
    PARITY_UNKNOWN = 0,
    PARITY_ODD = 1,
    PARITY_EVEN = 2
} PARITY;

typedef enum _DATA_FEATURE {
    DATA_FEATURE_NONE,
    DATA_FEATURE_CONVERT,
    DATA_FEATURE_TRANSLATE
} DATA_FEATURE;

char bcd_to_ebcdic(char, PARITY);
char ebcdic_to_bcd(char, PARITY);

int bcd_to_ebcdic_buffer(char *buffer, size_t bufferlen, PARITY parity);

void trtch_init(void);


#endif