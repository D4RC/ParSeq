#include "Reader.h"

char* read_reference()
{
    FILE *fp;
    long lsize;

    fp = fopen(REFERENCE_FILE,"rb");

    if(!fp)
    { 
        perror(REFERENCE_FILE);
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    lsize = ftell(fp);
    rewind(fp);

    char *reference = (char*)calloc(1, lsize+1);

    if(!reference)
    {
        fclose(fp);
        fputs("Reference memory allocaction fails", stderr);
        exit(1);
    }

    if(fread(reference, lsize, 1, fp)!=1)
    {
        fclose(fp);
        free(reference);
        fputs("Entire reference read fails", stderr);
        exit(1);
    }

    fclose(fp);
    return reference;
}