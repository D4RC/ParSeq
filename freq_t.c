#include <stdio.h>
#include <stdint.h>
#include <math.h>

#if UINTPTR_MAX == 0xffffffff
#define WLEN 4
#define WCONT 256
#elif UINTPTR_MAX == 0xffffffffffffffff
#define WLEN 8
#define WCONT 65536
#else
/* AHHHHH */
#endif

void fill_frequency_table(unsigned char *ref, int ref_size);
unsigned short freq_tab[WCONT][WLEN+1] = {0};

unsigned char *seq = "ACAAGATGCCATTGTCCCCCGGCCTCCTGCTGCTGCTGCTCTCCGGGGCCACGGCCACCGCTGCCCTGCCCCTGGAGGGTGGCCCCACCGGCCGAGACAGCGAGCATATGCAGGAAGCGGCAGGAATAAGGAAAAGCAGCCTCCTGACTTTCCTCGCTTGGTGGTTTGAGTGGACCTCCCAGGCCAGTGCCGGGCCCCTCATAGGAGAGGAAGCTCGGGAGGTGGCCAGGCGGCAGGAAGGCGCACCCCCCCAGCAATCCGCGCGCCGGGACAGAATGCCCTGCAGGAACTTCTTCTGGAAGACCTTCTCCTCCTGCAAATAAAACCTCACCCATGAATGCTCACGCAAGTTTAATTACAGACCTGAA";

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

    fill_frequency_table(seq, 369);

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
        freq_tab[row][WLEN] += 1;
    }
    printf("\n");
}