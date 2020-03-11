
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

/**
 * @brief add symbol to the list
 * @param lable the name of symbol
 * @param linkList the list of symbols
 * @param location the location
 * @param type whice type of symbol is it
 * @return if was or wasnt able to add symbol
 */
BOOL addSymbol(char *lable, PtSymbol linkList, MEMORY_TYPE location, symbolProperties type);



/**
 * @brief retunrs the symbol with givan name
 * @param symbolName name of symbol
 * @param root symbol list root
 * @return retunrs the symbol with givan name
 */
PtSymbol searchForSymbol(char *symbolName,PtSymbol root);
/**
 * @brief cheacks the symbol with parm and name
 * @param symbolName the name of thw symbol
 * @param root the root of list
 * @param paramter to be tested
 * @retun the Symbol
 */
PtSymbol searchForSymbolParm(char *symbolName,PtSymbol root,symbolProperties parm);

/**
 * @brief cheacks the next symbol with parm
 * @param root last symbol node
 * @param parm to be find
 * @return return the next node or NULL
 */
PtSymbol findNextSymbolWithParm(PtSymbol root,symbolProperties parm);


/**
 * @brief tests if giavn line has symbol or not
 * @param line to be tested
 * @return BOOL if has or not
 */
BOOL hasSymbol(char* line);

/**
 * @brief retunrs the symbol with givan name
 * @param symbol name of symbol
 * @param root symbol list root
 * @return retunrs the symbol with givan name
 */
PtSymbol isSymbol(char *symbol,PtSymbol root);




/**
 * @brief print symbols to file
 * @param root of symbol list
 * @param f  file to be saved to
 * @return
 */
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
/**
 * @brief this returns how many symbols with givin parm
 * @param root the root of the symbol list
 * @param parm the parm to be caounted
 * @return  the count of symbol with the givin parm
 */
int lenListWithParm(PtSymbol root,symbolProperties parm);

/**
 * @brief save the symbol to extrern list
 * @param line were the symbol saved
 * @param link current node of symbol list
 * @param IC current instruction counter
 * @return if Saved of not
 */
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
/**
 * @brief free the Symbol LinkList
 * @param root the root of the tree
 */
void freeList(PtSymbol root);
/**
 * @brief clears the Symbol LinkList
 * @param root the root of the tree
 */
void clearList(PtSymbol root);


#endif
