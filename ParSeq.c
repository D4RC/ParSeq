#include "Reader.h"
#include "P_LFMP.h"

void main()
{
    char *reference_seq;
    reference_seq = read_reference();

    fill_frequency_table(reference_seq, 369);
    for(int i=0; i<WCONT; i++)
    {
        for(int j=0; j<WLEN; j++)
        {   
            if(freq_tab[i][WLEN]>0)
                printf("%c",freq_tab[i][j]);
        }
        if(freq_tab[i][WLEN]>0)
        {
            printf(" %d ---",freq_tab[i][WLEN]);
            printf("\n");
        }
    }

    /*
    struct index_tracker windows = {NULL,0};
    struct index_tracker matches = {NULL,0};
  
    unsigned char *pattern = "CTGCTGCTG\0";
    unsigned int min_index;

    min_index = preprocessing(pattern, 9);
    printf("%d\n",min_index);
    windowing(min_index,reference_seq,369,pattern,9,&windows);
    printf("\nwindows %d\n", windows.count);
    matching(reference_seq,pattern,9,&windows,&matches);

    for(int i=0; i<matches.count; i++)
        printf("Found a match at index %d\n", matches.idx[i]);
        */


    free(reference_seq);
}