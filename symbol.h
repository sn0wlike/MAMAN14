
#ifndef MAMAN14_SYMBOL_H
#define MAMAN14_SYMBOL_H

#include "definitions.h"
#include "command.h"
#include "io_maneger.h"

#define ALREDY_USED_LABEL -1
#define NO_OPRANDS_OR_DATA_NEAR_LABLE -2

typedef struct _tSymbol tSymbol,*PtSymbol;


typedef enum{
    noType = 0,
    data,
    external,
    entry,
    code
} symbolProperties;

struct _tSymbol{
    char symbolName[MAX_LABLE_LENGTH];
    MEMORY_TYPE location;
    symbolProperties properties;
    PtSymbol next;
};





BOOL isExtern(char *oprand,PtSymbol root);

BOOL addSymbol(char *lable, PtSymbol linkList, MEMORY_TYPE location, symbolProperties type);

PtSymbol searchForSymbol(char *symbolName,PtSymbol root);
PtSymbol searchForSymbolParm(char *symbolName,PtSymbol root,symbolProperties parm);
PtSymbol findNextSymbolWithParm(PtSymbol root,symbolProperties parm);



BOOL hasSymbol(char* line);
PtSymbol isSymbol(char *symbol,PtSymbol root);





BOOL printSymbols(PtSymbol root,FILE *f);

/**
 * this funvtion update the symbol of the data image symbols
 * @param root root node of symbol link list
 * @param pos the postion where the data should be saved from
 */
void updateSymbolData (PtSymbol root, UINT pos);

/**
 * update the entry location of the code to be enterd for the entry poiny
 * @param dis entry list
 * @param src symbol list
 * @return if FALSE there is undefined Entry point
 */
BOOL updateSymbolEntry (PtSymbol dis,PtSymbol src);


int lenList(PtSymbol root);
int lenListWithParm(PtSymbol root,symbolProperties parm);
BOOL saveSymbolExtern(char* line, PtSymbol link, UINT IC);

/**
 * @param symbol linklist distantion node to save to
 * @param eFlags struct of the errors wil be saved and raised
 * @param flags flags that should be rasied in case of error
 * @param line line to saved and parsed as symbol
 * @param position postion of symbol in the asm code
 * @param type is it symbol to data code or entry/external
 * @return  nex symbol node is the linklist
 */
PtSymbol saveSymbol(PtSymbol symbol, pE_Flag eFlags, pFlag flags, char *line, UINT position, symbolProperties type);

void freeList(PtSymbol root);
void clearList(PtSymbol root);


#endif
