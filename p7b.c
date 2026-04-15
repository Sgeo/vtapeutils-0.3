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

int p7b_read(VTAPE_FILE *infile, unsigned char *buffer, unsigned int maxlen)
{
    size_t position = 0;
    char current;
    if(fread(&current, 1, 1, infile->file) < 1) {
        return -1;
    }
    if(!(current & P7B_START)) {
        printf("P7B Error: Record doesn't start with start record mark.\n");
        return -1;
    }
    current &= ~P7B_START;
    //current &= ~P7B_PARITY; // TODO: Check parity?

    if((current&~P7B_PARITY) == P7B_TAPE_MARK) {
        while((current&~P7B_PARITY) == P7B_TAPE_MARK) {
            if(fread(&current, 1, 1, infile->file) < 1) {
                if(feof(infile->file)) {
                    // Consider EOF after tape mark to be valid
                    return 0;
                } else {
                    return -1;
                }
            }
        }
        // Back up 1 since we presumably overshot
        if(fseek(infile->file, -1, SEEK_CUR)!=0) {
            return -1;
        }
        return 0;
    }

    buffer[position++] = current;

    while(1) {
        if(position >= maxlen) {
            printf("Record too long!\n");
            return -1;
        }
        position++;
        if(fread(&current, 1, 1, infile->file) < 1) {
            return -1;
        }
        //current &= ~P7B_PARITY;
        if(current & P7B_START) {
            if(fseek(infile->file, -1, SEEK_CUR)!=0) {
                return -1;
            }
            position--;
            return position;
        }
        buffer[position] = current;
    }

    

}

int p7b_write(VTAPE_FILE *outfile, unsigned char *buffer, unsigned int reclength)
{
    printf("Writing P7B is unsupported and probably unsupportable.\n");
    return -1;
}