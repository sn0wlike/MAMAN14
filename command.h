#ifndef MAMAN14_COMMAND_H
#define MAMAN14_COMMAND_H

#include "definitions.h"
#include "symbol.h"
#include "io_maneger.h"



#define INSTRACTION_OPCODE_BIT_LOCATION 11
#define INSTRACTION_ADDRESS_SRC_METHOD 7
#define INSTRACTION_ADDRESS_DIS_METHOD 3
#define INSTRACTION_OPCODE_ARE_LOCATION 0
#define OPRAND_ADDRESS_LOCATION 3


#define OPRAND_AMOUNT 16
#define OPRAND_LIST {"mov","cmp","add","sub",\
                    "lea","clr","not","inc",\
                    "dec","jmp","bne","red",\
                    "prn","jsr","rts","stop"};

#define NO_COMMAND_FOUND -1

#define LINE_PARSED 0
#define MISSING_COMA -3

/**
 * struct to describe the structre of any given instruction
 * cmd - instructionOpcode
 * src - string of src
 * dis - string of dis
 * oprandsCount - how much oprands has this instruction
 */
typedef struct {
    unsigned int cmd:4;
    char* src;
    char* dis;
    unsigned int oprandsCount:2;
}instruction,*pInstruction;

/**
 * @brief this will parse from given text to instruction struct
 * @param line text to be parsed
 * @param Command distention struct
 * @return will return status of successes
 */
int instructionSplitter(char* line, pInstruction Command);

/**
 * @brief return the size of given instruction in
 * @param instruction1 instruction to be sized
 * @return size of the instruction
 */
int sizeOfInstruction(pInstruction instruction1);

/**
 * @brief this function returns if the giving text has an instruction
 * @param line the txt to be chhecked
 * @return return -1 if noFound or non negative number
 */
char hasInstruction(char* line);

#endif
