#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#if UINTPTR_MAX == 0xffffffff
#define WLEN 4
#define WCONT 256
#elif UINTPTR_MAX == 0xffffffffffffffff
#define WLEN 8
#define WCONT 65536
#else
/* AHHHHH */
#endif

struct index_tracker {
    unsigned int *idx;
    unsigned int count;
};

void fill_frequency_table(unsigned char *ref, int ref_size);
int preprocessing(unsigned char *pattern, unsigned int pattern_size);
void windowing(int min_index, unsigned char *text, unsigned int text_size, unsigned char *pattern, unsigned int pattern_size, struct index_tracker *windows);
void matching(unsigned char *text, unsigned char *pattern, unsigned int pattern_size, struct index_tracker *windows, struct index_tracker *matches);

static unsigned short freq_tab[WCONT][WLEN+1] = {0};
static unsigned char *reference_seq = "ACAAGATGCCATTGTCCCCCGGCCTCCTGCTGCTGCTGCTCTCCGGGGCCACGGCCACCGCTGCCCTGCCCCTGGAGGGTGGCCCCACCGGCCGAGACAGCGAGCATATGCAGGAAGCGGCAGGAATAAGGAAAAGCAGCCTCCTGACTTTCCTCGCTTGGTGGTTTGAGTGGACCTCCCAGGCCAGTGCCGGGCCCCTCATAGGAGAGGAAGCTCGGGAGGTGGCCAGGCGGCAGGAAGGCGCACCCCCCCAGCAATCCGCGCGCCGGGACAGAATGCCCTGCAGGAACTTCTTCTGGAAGACCTTCTCCTCCTGCAAATAAAACCTCACCCATGAATGCTCACGCAAGTTTAATTACAGACCTGAA";

int int_pow(int b, int e)
{
    int r = 1;
    for(int i=0; i<e; i++)
        r*=b;
    return r;
}

void main()
{
    unsigned char alphabet[] = {'A','C','G','T'};

    unsigned int multi[] = {16384,4096,1024,256,64,16,4,1};
    for(int i=0; i<WLEN; i++)
    {
        for(int j=0; j<WCONT; j++)
        {
            freq_tab[j][i] = alphabet[(j/multi[i])%4];
        }
    }

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

}

void fill_frequency_table(unsigned char *ref, int ref_size)
{
    // Iterate words
    for(int i = 0; i < ref_size - WLEN; i++)
    {
        int row = 0;
        // Iterate characters
        for(int j=0; j < WLEN; j++)
        {
            printf("%c", ref[i+j]);
            switch(ref[i+j])
            {
                case 'A':
                    break;
                case 'C':
                    row += 1 * int_pow(4,WLEN-j-1);
                    break;
                case 'G':
                    row += 2 * int_pow(4,WLEN-j-1);
                    break;
                case 'T':
                    row += 3 * int_pow(4,WLEN-j-1);
                    break;
            }
        }
        freq_tab[row][WLEN]++;
    }
    printf("\n");
}

/*
    @brief  Finds the least frequent word in the pattern
*/
int preprocessing(unsigned char *pattern, unsigned int pattern_size)
{
    int min_value = INT32_MAX;
    int min_index = -1;

    for(int i=0; i < (pattern_size - WLEN); i++)
    {
        int row = 0;
        for(int j=0; j < WLEN; j++)
        {
            switch (pattern[i+j])
            {
                case 'A':
                    break;
                case 'C':
                    row += 1 * int_pow(4, WLEN -j -1);
                    break;
                case 'G':
                    row += 2 * int_pow(4, WLEN -j -1);
                    break;
                case 'T':
                    row += 3 * int_pow(4, WLEN -j -1);
                    break;
            }
        }
        if(freq_tab[row][WLEN] < min_value)
        {
            min_value = freq_tab[row][WLEN];
            min_index = i;
        }
    }
    return min_index;
}

/*
    Defines windows in which to search
*/
void windowing(int min_index, unsigned char *text, unsigned int text_size, unsigned char *pattern, unsigned int pattern_size, struct index_tracker *windows)
{
    for(int i=min_index; i<text_size-(pattern_size-min_index); i++)
    {
        int equal = 1;
        for(int j=0; j<WLEN; j++)
        {
            if(text[i+j]!=pattern[min_index+j])
                equal = 0;
        }
        if(equal)
        {
            windows->idx = (unsigned int*)realloc(windows->idx, windows->count+1);
            windows->idx[windows->count] = i - min_index;
            windows->count++;
        }
    }
}

/*
    Returns the first indexes for all ocurrences of pattern 
*/
void matching(unsigned char *text, unsigned char *pattern, unsigned int pattern_size, struct index_tracker *windows, struct index_tracker *matches)
{
    int num_match = 0;
    int k = pattern_size % WLEN;

    for(int i=0; i<windows->count; i++)
    {
        int s = windows->idx[i];
        int w = WLEN;

        int c = 0;
        for(c = 0; c<pattern_size; c+=w)
        {
            if(c > pattern_size - WLEN)
            {
                w = k;
            }
            
            int nequal = 0;
            for(int j=c; j<w; j++)
            {
                if(pattern[c] != text[s+c])
                    nequal = 1;
            }

            if(nequal)
                break;
        }

        if(c==pattern_size)
        {
            matches->idx = (unsigned int*)realloc(matches->idx, matches->count+1);
            matches->idx[matches->count] = s;
            matches->count++;
        }
    }
}