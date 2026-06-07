// Sources of information on P7B:
// https://simh.trailing-edge.com/sources/simtools/extracters/mtdump.txt
// https://github.com/open-simh/simh/blob/master/sim_tape.h
// https://github.com/open-simh/simh/blob/master/sim_tape.c
// https://www.piercefuller.com/oldibm-shadow/tool.html

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vtape.h"
#include "p7b.h"
#include "trtch.h"

int fgetc_skip_null(FILE* stream)
{
  int result = 0;
  do {
    result = fgetc(stream);
  } while(result == 0x00);
  return result;
}

int p7b_open(VTAPE_FILE *file, char *filename, char *mode)
{
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int p7b_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}

int p7b_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen, PARITY *parity)
{
    size_t position = 0;
    int current_int;
    char current_char;
    unsigned char block_parity = 0;
    int tapemark = 0;
    
    current_int = fgetc_skip_null(infile->file);
    if(current_int < 0) return -1;

    current_char = current_int;

    if(!(current_char&P7B_START))
    {
        printf("Non-START at beginning of file!\n");
        return -1;
    }
    current_char &= ~P7B_START;
    block_parity = parity_of_byte(current_char);
    if(parity != NULL) {
      *parity = (block_parity == 1) ? PARITY_ODD : (block_parity == 2) ? PARITY_EVEN : PARITY_UNKNOWN;
    }

    if((current_char&~P7B_PARITY)==P7B_TAPE_MARK)
    {
        // Special tapemark reading mode. The spec allows for a second redundant tapemark character
        tapemark = 1;
    }

    while(!(current_char&P7B_START))
    {
        if(block_parity != parity_of_byte(current_char)) {
          printf("Inconsistent parity detected in record!\n");
        }
        current_char &= ~P7B_PARITY;
        if(!tapemark)
        {
            if(position >= maxlen) {
              return -1;
            }
            buffer[position++] = current_char;
        }
        current_int = fgetc_skip_null(infile->file);
        if(current_int < 0) return position;
        current_char = current_int;
    }
    // current_int and current_char should be a start
    if(ungetc(current_int, infile->file)==EOF) return -1;

    // position in this point is how many bytes read

    return position;

    

}

int p7b_write(VTAPE_FILE *outfile, unsigned char *buffer, unsigned int reclength)
{
    printf("Writing P7B is unsupported and probably unsupportable.\n");
    return -1;
}