

#ifndef MAMAN14_OBJECT_H
#define MAMAN14_OBJECT_H

#include "definitions.h"
#include "command.h"
#include "symbol.h"

#define IMMEDIATE_OPRAND_PASS 1
#define DIRECT_OPRAND_PASS 2
#define NON_IMMEDIATE_REGISTER_PASS 4
#define DIRECT_REGISTER_PASS 8

#define UNKNOW_OPRAND -1
#define ADRESSING_ERROR -2
#define OB_LINE_CREATED 0

#define RELATIVE_ARE 2
#define ABSOLUTE_ARE 4
#define EXTERNAL_ARE 1

typedef struct _tObject object,*pObject;
struct _tObject{
    UINT ob:15;
};

/**
 *
 * @brief this function prints to file the object crated
 * @param f file to print the Object crated
 * @param ins the instruction
 * @param IC  instruction counter of current line
 * @return Error Code
 */
int crateOBline(FILE* f, pInstruction ins, UINT IC);
/**
 * @brief this function identifies the adressing method
 * @param methode this will have the method
 * @param oprand the oprend to identify the mthod from
 * @return Error code
 */
int adreesMathodidentifier(UCHAR* methode,char* oprand);


#endif
