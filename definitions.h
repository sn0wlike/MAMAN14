
#ifndef MAMAN14_DEFINITIONS_H
#define MAMAN14_DEFINITIONS_H

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>




#define SHIFT_TO_NUMBER(a) a-0x30
#define SHIFT_BY(a) (int)pow(2,a)

#define UCHAR  unsigned char
#define UINT  unsigned int

#define BOOL UINT
#define FALSE 0
#define TRUE !FALSE

#define NON_INTEGER -1
#define OUT_OF_RANGE -2
#define NO_FILES_INPUT_ERROR "too few arguments passed"
#define NO_COMPITABLE_EXTENSION "this file: %s\n extention isn't compatibale\n",*(argv + fileIndexer)
#define NO_STRING_TERMANIATOR -4;
#define MEMORY_TYPE UINT
#define OPRAND_MAX_VALUE 15
#define OPRAND_MIN_VALUE 0

#define START_INSTRUCTION_LOCATION (UINT) 100
#define MAX_LINE (UINT)80+1+1 /*MAX LINE + \n + \0*/
#define MAX_MEMORY_SIZE (UINT)4096
#define MIN_NUMBER_VALUE_NOT_DATA -4097
#define MAX_NUMBER_VALUE_NOT_DATA 4096

#define SPACES_ARRAY {'\n','\t',' '}
#define OPRAND_LIST {"mov","cmp","add","sub",\
                    "lea","clr","not","inc",\
                    "dec","jmp","bne","red",\
                    "prn","jsr","rts","stop"};
#define REGISTER_LIST {"r0","r1","r2","r3","r4",\
                        "r5","r6","r7"}
#define SAVE_WORDS_ETC_LIST {".data",".string",".entry",".extern"};
#define REGISTER_LIST_LEN 8
#define SAVE_WORDS_ETC_LIST_LEN 4
#define SPACES_ARRAY_SIZE 3
#define OPRAND_AMOUNT 16

#define DATA_INSTRUCTION ".data "
#define STRING_INSTRUCTION ".string "
#define EXTERN_INSTRUCTION ".extern "
#define ENTRY_INSTRUCTION ".entry "

#define MAX_LABLE_LENGTH 31+1


#define NULL_DALIMTER_STRING "000000000000000\n"

#define MSB_ON 0x4000
#define DATA_SIZE_ARRAY 15+1+1

#define MAX_DIRECTORY_LEN 255+1
#define FILE_EXTENSION_INPUT ".as"
#define FILE_EXTENSION_OUTPUT ".ob"
#define FILE_EXTENSION_TMP ".tmp"
#define FILE_EXTENSION_EXT ".ext"
#define FILE_EXTENSION_ENT ".ent"



typedef struct{
    UINT isSymbolInLine:1;
    UINT dontMakeFile:1;
}flag,*pFlag;


typedef struct{
    UINT symbolExsitis:1;
    UINT unknownInstrction:1;
    UINT errorInTheData:1;
    UINT addressingError:1;
    UINT unknowOprand:1;
    UINT unknowError:1;
    UINT undefeindedEntrySymbol:1;
    UINT unknowInteger:1;
    UINT unableToCrateFile:1;
    UINT tooLongLine:1;
    UINT noStringEnd:1;

}e_flags,*pE_Flag;

/**
 * this function will print any error raised by eFlags
 * @param eFlag the source of errors
 * @param flag1 flags of saving files
 * @param lineCounter line of the error occured
 * @return if there was any error
 */
BOOL printError(pE_Flag eFlag, pFlag flag1, int lineCounter);






#endif
