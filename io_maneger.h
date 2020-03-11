

#ifndef MAMAN14_IO_MANEGER_H
#define MAMAN14_IO_MANEGER_H

#include "definitions.h"


/*
 * this function is for reading an unknown size of tring
 * it need any inputstrem(FILE or stdin)
 * the size is for minimus starting size for the realloc
 * */
char *readLine(FILE* fp, size_t size);


/**
 *
 * @param data
 * @param parm
 */
void clrSpaces(char* data);

BOOL printToDataFile(char *line, FILE *f, UINT *DC, pE_Flag errorFlag);

/**
 * this will paste the data img to the output file
 * @param fObj output file
 * @param fData data src file
 * @param IC the postion to start from
 */
void insertDataToObj(FILE *fObj, FILE *fData, UINT IC);

void oct(FILE*f,unsigned n);

BOOL isEmptyLine(char *Line);
BOOL isNoteLine(char *line);

void makeFileName(char *nameFile, char *inputFileName, char *extension);

BOOL fileIsLegit(char *filename);


#endif
