// BCD <-> EBCDIC conversions based on https://bitsavers.org/pdf/ibm/2400/GA22-6866-5_2400-Series_CompDescr_Jul71.pdf
// https://bitsavers.org/pdf/ibm/2415/Y22-2918-0_2415_Models_1-3_Tape_Control_FETO_1966.pdf for substitute blank behavior

// cc == collating code

#include "trtch.h"
#include <stdio.h>

unsigned char cc_to_ebcdic[64] = {
    0x40, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x5B,
    0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x6B, 0x6C,
    0x6D, 0x6E, 0x6F, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E,
    0x7F, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6,
    0xC7, 0xC8, 0xC9, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4,
    0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xE0, 0xE2, 0xE3,
    0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xF0, 0xF1,
    0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9
};

unsigned char cc_to_bcd[64] = {
    000, 073, 074, 075, 076, 077, 060, 053,
    054, 055, 056, 057, 040, 021, 033, 034,
    035, 036, 037, 020, 013, 014, 015, 016,
    017, 072, 061, 062, 063, 064, 065, 066,
    067, 070, 071, 052, 041, 042, 043, 044,
    045, 046, 047, 050, 051, 032, 022, 023,
    024, 025, 026, 027, 030, 031, 012, 001,
    002, 003, 004, 005, 006, 007, 010, 011
};

unsigned char bcd_to_ebcdic_table[64] = { 0 };
unsigned char ebcdic_to_bcd_table[256] = { 0 };

void trtch_init(void) {
    for(int i = 0; i < 64; i++) {
        unsigned char ebcdic = cc_to_ebcdic[i];
        unsigned char bcd = cc_to_bcd[i];
        bcd_to_ebcdic_table[bcd] = ebcdic;
        ebcdic_to_bcd_table[ebcdic] = bcd;
    }
}

char bcd_to_ebcdic(char bcd, PARITY parity) {
    if((parity == PARITY_EVEN) && (bcd == 020)) {
        return 0x40;
    }
    return bcd_to_ebcdic_table[bcd];
}