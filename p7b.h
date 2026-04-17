#define P7B_PARITY 0x40
#define P7B_START 0x80
#define P7B_TAPE_MARK  0x0F


/* Functions. */
int p7b_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen, DATA_FEATURE data_feature);
int p7b_write(VTAPE_FILE *outfile, unsigned char *buffer, unsigned int reclength, DATA_FEATURE data_feature);
int p7b_open(VTAPE_FILE *file, char *filename, char *mode);
int p7b_close(VTAPE_FILE *file);
