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
  trtch_init();
  if ((file->file = fopen(filename,mode)) == NULL) return -1;
  return 0;
}

int p7b_close(VTAPE_FILE *file)
{
  if (fclose(file->file) == EOF) return -1;
  return 0;
}

int p7b_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen, DATA_FEATURE data_feature)
{
    size_t position = 0;
    int current_int;
    char current_char;
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
    current_char &= ~P7B_PARITY; // Possibly controversial, but the 9-track containers don't store parity.

    if(current_char==P7B_TAPE_MARK)
    {
        // Special tapemark reading mode. The spec allows for a second redundant tapemark character
        tapemark = 1;
    }

    while(!(current_char&P7B_START))
    {
        current_char &= ~P7B_PARITY;
        if(data_feature == DATA_FEATURE_TRANSLATE)
        {
          current_char = bcd_to_ebcdic(current_char, PARITY_EVEN);
        }
        if(!tapemark)
        {
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

int p7b_write(VTAPE_FILE *outfile, unsigned char *buffer, unsigned int reclength, DATA_FEATURE data_feature)
{
    printf("Writing P7B is unsupported and probably unsupportable.\n");
    return -1;
}