#ifndef __READER_H__
#define __READER_H__

#include <stdio.h>
#include <stdlib.h>

#define REFERENCE_FILE "reference.txt"

/* Returns a pointer to a buffer with the reference sequence */
char* read_reference();

#endif