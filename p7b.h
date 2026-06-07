#define P7B_PARITY 0x40
#define P7B_START 0x80
#define P7B_TAPE_MARK  0x0F

#include "trtch.h"


/* Functions. */
int p7b_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen, PARITY *parity);
int p7b_write(VTAPE_FILE *outfile, unsigned char *buffer, unsigned int reclength);
int p7b_open(VTAPE_FILE *file, char *filename, char *mode);
int p7b_close(VTAPE_FILE *file);
