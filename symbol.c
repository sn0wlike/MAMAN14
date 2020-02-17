
#include "symbol.h"


BOOL isSymbolExsits(char* label,PtSymbol node,symbolProperties type)
{
    if(type == external || type == entry)
    {
        return FALSE;
    }
    if(node == NULL)
        return FALSE;
    if(strcmp(label, node->symbolName) == 0)
    {
        return TRUE;
    }else
        return isSymbolExsits(label, node->next,type);

}

tSymbol symbolListRoot;
tSymbol EsymbolListRoot;

PtSymbol isSymbol(char *symbol,PtSymbol root)
{
    if(root == NULL)
        return NULL;
    if(strcmp(symbol,root->symbolName) == 0)
    {
        return root;
    }else
    {
        return isSymbol(symbol,root->next);
    }
}

PtSymbol searchForSymbol(char *symbolName,PtSymbol root)
{
    if(root == NULL || root->next == NULL)
    {
        return NULL;
    }
    if(strcmp(symbolName,root->symbolName) == 0)
    {
        return root;
    }else
    {
        return searchForSymbol(symbolName,root->next);
    }
}

PtSymbol searchForSymbolParm(char *symbolName, PtSymbol root, symbolProperties parm) {
    if(root == NULL || root->next == NULL)
    {
        return NULL;
    }
    if((strcmp(symbolName,root->symbolName) == 0)
       &&
       (root->properties == parm))
    {
        return root;
    }else
    {
        return searchForSymbol(symbolName,root->next);
    }
}

BOOL hasSymbol(char *line) {
    if(strchr(line,':') != NULL)
        return TRUE;
    return FALSE;
}

BOOL addSymbol(char *lable, PtSymbol linkList, MEMORY_TYPE location, symbolProperties type) {


    char* endOfSymbol;
    char* pSymbol = linkList->symbolName;
    linkList->location = location;
    linkList->properties = type;
    endOfSymbol = strchr(lable,':');
    if(endOfSymbol!=NULL)*endOfSymbol = '\0';
    /*cheack double only if isn't entry or external*/
    if(type != external && type != entry && isSymbolExsits(lable,&symbolListRoot,type))
        return FALSE;
    strcpy(pSymbol,lable);
    if(endOfSymbol!=NULL)*endOfSymbol = ':';
    linkList->next = malloc(sizeof(tSymbol));
    linkList->next->location = 0;
    linkList->next->properties = 0;
    linkList->next->next = NULL;
    strcpy(linkList->next->symbolName," ");

    return TRUE;


}

void printSymbol(PtSymbol node,FILE *f)
{
    fprintf(f,"name|\t%s\t|location|\t%d\t|type|\t%d\t\n",node->symbolName,node->location,node->properties);
}

BOOL printSymbols(PtSymbol root,FILE *f)
{
    printSymbol(root,f);
    if(root->next != NULL) {
        printSymbols(root->next,f);
        return 1;
    }
    return 0;
}

void updateSymbolData (PtSymbol root, UINT pos){
    if(root == NULL)
    {
        return ;
    }

    if(root->properties == data)
    {
        root->location = root->location+pos;
    }
    updateSymbolData(root->next,pos);
}

BOOL updateSymbolEntry (PtSymbol dis,PtSymbol src)
{
    dis = findNextSymbolWithParm(dis,entry);
    if(dis != NULL) {
        PtSymbol tmp = searchForSymbol(dis->symbolName, src);
        if(!tmp)
        {
            return FALSE;
        }
        dis->location = tmp->location;

        if(dis->next != NULL)
        {
            return updateSymbolEntry(dis->next,src);
        }else
        {
            return TRUE;
        }
    }
    return TRUE;
}

int lenList(PtSymbol root) {

    if(root->next == NULL) {
        return 0;
    }
    else
    {
        return 1 +lenList(root->next);
    }
}

int lenListWithParm(PtSymbol root,symbolProperties parm) {

    if(root->next == NULL) {
        return 0;
    }
    else
    {
        if(root->properties == parm)
            return 1 +lenListWithParm(root->next,parm);
        else
            return lenListWithParm(root->next,parm);
    }
}

PtSymbol findNextSymbolWithParm(PtSymbol root,symbolProperties parm){
    if(root == NULL)
    {
        return NULL;
    }else
    {
        if(root->properties == parm)
        {
            return root;
        }else
        {
            if(root->next != NULL)
                return findNextSymbolWithParm(root->next,parm);
            else
                return NULL;
        }
    }
}

BOOL saveSymbolExtern(char* line, PtSymbol link, UINT IC)
{
    char *entry1,*ext;
    ext = strstr(line,EXTERN_INSTRUCTION);
    entry1 = strstr(line,ENTRY_INSTRUCTION);
    if( entry1 != NULL && ext != NULL)
        return FALSE;
    else{
        if(ext != NULL)
        {
            ext = strchr(ext,' ');
            clrSpaces(ext);
            return addSymbol(ext,link,IC,external);
        }else{
            entry1 = strchr(entry1,' ');
            clrSpaces(entry1);
            return addSymbol(entry1,link,IC,entry);
        }
    }
 }

BOOL isExtern(char *oprand, PtSymbol root) {
    if(searchForSymbolParm(oprand,root,external))
        return TRUE;
    else
        return FALSE;
}

PtSymbol saveSymbol(PtSymbol symbol, pE_Flag eFlags, pFlag flags, char *line, UINT position, symbolProperties type) {
    eFlags->symbolExsitis = !addSymbol(line, symbol, position, type);
    flags->isSymbolInLine = FALSE;
    return symbol->next;


}

void freeList(PtSymbol root) {
    if(root == NULL)
    {
        return;;
    }
    freeList(root->next);
    free(root);
}

void clearList(PtSymbol root) {
    freeList(root->next);
    root->next = NULL;
    root->properties = noType;
    strcpy(root->symbolName,"");
    root->location = 0;
}
