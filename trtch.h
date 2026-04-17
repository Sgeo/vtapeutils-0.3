typedef enum _PARITY {
    PARITY_UNKNOWN = 0,
    PARITY_ODD = 1,
    PARITY_EVEN = 2
} PARITY;

char bcd_to_ebcdic(char, PARITY);
char ebcdic_to_bcd(char, PARITY);

void trtch_init(void);