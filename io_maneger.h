

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

/**
 * @brief print octal value of number to file
 * @param f FILE* of the distantion file
 * @param n number to be printed
 */
void oct(FILE*f,unsigned n);

/**
 * @brief return if th line is empty
 * @param Line line to be tested
 * @return is empty or not
 */
BOOL isEmptyLine(char *Line);

/**
 * @brief return of the line is note line
 * @param line line to be tested
 * @return is not line or not
 */
BOOL isNoteLine(char *line);

/**
 * @brief crates file name by extenstion
 * @param nameFile output file name
 * @param inputFileName the source file
 * @param extension extension for the new file
 */
void makeFileName(char *nameFile, char *inputFileName, char *extension);


/**
 * @brief tests if the file have legit extension
 * @param filename the file name to be tested
 * @return if the file legit or not
 */
BOOL fileIsLegit(char *filename);


#endif
