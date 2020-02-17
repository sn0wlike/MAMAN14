

#ifndef MAMAN14_OBJECT_H
#define MAMAN14_OBJECT_H

#include "definitions.h"
#include "command.h"
#include "symbol.h"

#define IMMEDIATE_OPRAND_PASS 1
#define DIRECT_OPRAND_PASS 2
#define NON_IMMEDIATE_REGISTER_PASS 4
#define DIRECT_REGISTER_PASS 8

#define OPCODE_START_BIT 11
#define REGISTER_LIST {"r0","r1","r2","r3","r4",\
                        "r5","r6","r7"}
#define REGISTER_LIST_SIZE 8

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
typedef struct{
    UINT methode:4;
}adreesMathod,*pAdressmathod;

BOOL addOB(pObject head,int ob);
pObject newOB();

int crateOBline(FILE* f, pInstruction ins, UINT IC);
int adreesMathodidentifier(UCHAR* methode,char* oprand);


#endif
