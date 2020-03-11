#include <stdio.h>

#include <unistd.h>
#include "definitions.h"
#include "command.h"
#include "io_maneger.h"
#include "object.h"

extern tSymbol symbolListRoot;
extern tSymbol EsymbolListRoot;

void printToExternalFile(FILE *output, char *symbol, UINT IC);

void printToExternal(pInstruction instruction1, UINT IC, FILE* outPutExt);

BOOL printEntryFile(FILE *outPutFILE, PtSymbol root);

BOOL printError(pE_Flag eFlag, pFlag flag1, int lineCounter);

BOOL phaseOne(char *srcFileName, pFlag flags, pE_Flag eFlags, UINT *DC, UINT *IC);

BOOL phaseTwo(char *srcFileName, pFlag flags, pE_Flag eFlags, UINT *IC);


int main(int argc,char** argv) {
    /**
     * algorithm:
     * 1. parse lables
     * 2. parse instruction/guide
     * 3. convert to command
     * 4.export to file
     */
    e_flags eFlags = {0, 0,
                      0,0,
                      0,0,
                      0,0,
                      0,};
    flag flags = {0, 0};

    UINT IC = 0;
    UINT DC = 0;
    UINT fileIndexer = 1;

    /*TODO: cheack if src file end with .as*/


    if(argc == 1) {
        printf(NO_FILES_INPUT_ERROR);
    }

    while(fileIndexer <= (argc - 1)) {
        if(!fileIsLegit(*(argv + fileIndexer)))
        {
            printf(NO_COMPITABLE_EXTENSION);
        }else {
            printf("Working on %s:\nLOG:\n",*(argv + fileIndexer));
            if(phaseOne(*(argv + fileIndexer), &flags, &eFlags, &DC, &IC)) {
                IC = 0;
                phaseTwo(*(argv + fileIndexer), &flags, &eFlags, &IC);
            }
        }
        fileIndexer++;
        clearList(&EsymbolListRoot);
        clearList(&symbolListRoot);
        IC = 0;
        DC = 0;
    }
    return 0;
}



BOOL phaseOne(char *srcFileName, pFlag flags, pE_Flag eFlags, UINT *DC, UINT *IC) {
    int lineCounter = 0;
    char dataTmpFileName[MAX_DIRECTORY_LEN] = {0};
    char outPutFileName[MAX_DIRECTORY_LEN] = {0};
    char line[MAX_LINE] = {0};

    instruction instruction1;

    PtSymbol symbol = &symbolListRoot;
    PtSymbol Esymbol = &EsymbolListRoot;
    FILE *sourceFile = fopen(srcFileName, "r");
    FILE *dataImgTmp;
    FILE *outPutOb;


    makeFileName(dataTmpFileName,srcFileName,FILE_EXTENSION_TMP);
    makeFileName(outPutFileName,srcFileName,FILE_EXTENSION_OUTPUT);

    dataImgTmp = fopen(dataTmpFileName, "w");
    outPutOb = fopen(outPutFileName, "w");

    if (sourceFile == NULL || dataImgTmp == NULL || outPutOb == NULL) {
        flags->dontMakeFile = TRUE;
        eFlags->unableToCrateFile = TRUE;
        goto FINISH;
    }
    while (fgets(line, MAX_LINE, sourceFile) != NULL) {
        if( *(line+strlen(line)-1) != '\n' || strlen(line) >= MAX_LINE || isNoteLine(line) || isEmptyLine(line))
        {
            if(strlen(line) >= MAX_LINE )
            {
                eFlags->tooLongLine = TRUE;
                flags->dontMakeFile = TRUE;
            }
            goto SKIP;
        }
        /*here I'll raise a flag if there is any symbol definition inline*/
        if (hasSymbol(line)) {
            flags->isSymbolInLine = TRUE;
        }
        /*if line has any instruction and not a command*/
        if (strstr(line, DATA_INSTRUCTION) || strstr(line, STRING_INSTRUCTION)) /*LINE 5*/
        {
            /*if it's a data of string ill save it in a tmp file of the data img*/
            if (flags->isSymbolInLine) {
                /*add symbol to symbol list with the type data */
                symbol = saveSymbol(symbol, eFlags, flags, line, *DC, data);
            }
            eFlags->errorInTheData = !printToDataFile(line, dataImgTmp, DC, eFlags);
        } else if (strstr(line, EXTERN_INSTRUCTION) || strstr(line, ENTRY_INSTRUCTION)) /*LINE 8*/
        {
            /*print entery external Error*/
            printf("WARNING: using ");
            if (flags->isSymbolInLine) {
                /*add symbol to symbol list with the type entry */
                symbol = saveSymbol(symbol, eFlags, flags, line, *DC, entry);
            }
            if (strstr(line, EXTERN_INSTRUCTION) != NULL) {
                printf("external ");

            } else {
                printf("entry ");
            }
            printf("maight not be defined inline:%d\n", lineCounter + 1);
            saveSymbolExtern(line, Esymbol, *IC + START_INSTRUCTION_LOCATION);
            Esymbol = Esymbol->next;
        } else {/*LINE 11*/
            if (flags->isSymbolInLine) {
                /*add symbol to symbol list with the type code */
                symbol = saveSymbol(symbol, eFlags, flags, line, *IC + START_INSTRUCTION_LOCATION, code);
            }
            eFlags->unknownInstrction = (hasInstruction(line) >= 0) ? FALSE : TRUE;
            instructionSplitter(line, &instruction1);
            *IC = *IC + sizeOfInstruction(&instruction1);

            instruction1.cmd = 0;
            instruction1.oprandsCount = 0;
            instruction1.dis = NULL;
            instruction1.src = NULL;
        }

        SKIP:

        lineCounter++;
        printError(eFlags, flags, lineCounter);
    }


    updateSymbolData(&symbolListRoot, *IC + START_INSTRUCTION_LOCATION);
    eFlags->undefeindedEntrySymbol = !updateSymbolEntry(&EsymbolListRoot, &symbolListRoot);
    FINISH:
    /*ill print to putput file size of code and size of data*/
    if (outPutOb)
        fprintf(outPutOb, "%d %d\n", *IC, *DC);

    if (flags->dontMakeFile || (*IC+START_INSTRUCTION_LOCATION) >= MAX_MEMORY_SIZE) {
        remove(outPutFileName);
        remove(dataTmpFileName);
    }

    if(sourceFile != NULL)fclose(sourceFile);
    if(dataImgTmp!= NULL)fclose(dataImgTmp);
    if(outPutOb!= NULL)fclose(outPutOb);


    printError(eFlags, flags, lineCounter);

    return !flags->dontMakeFile;

}

BOOL phaseTwo(char *srcFileName, pFlag flags, pE_Flag eFlags, UINT *IC) {
    int lineCounter = 0;
    int result;
    char *splitter = NULL;

    char dataTmpFileName[MAX_DIRECTORY_LEN] = {0};
    char outPutFileName[MAX_DIRECTORY_LEN] = {0};
    char outPutExtFileName[MAX_DIRECTORY_LEN] = {0};
    char outPutEntFileName[MAX_DIRECTORY_LEN] = {0};
    char line[MAX_LINE] = {0};

    FILE *inputFile = NULL;
    FILE *dataImgTmp = NULL;
    FILE *outPutOb = NULL;
    FILE *outPutExt = NULL;
    FILE *outPutEnt = NULL;

    instruction instruction1 = {0,NULL,NULL,0,};

    makeFileName(dataTmpFileName,srcFileName,FILE_EXTENSION_TMP);
    makeFileName(outPutFileName,srcFileName,FILE_EXTENSION_OUTPUT);
    makeFileName(outPutExtFileName,srcFileName,FILE_EXTENSION_EXT);
    makeFileName(outPutEntFileName,srcFileName,FILE_EXTENSION_ENT);

/**
 * openning and creating files
 */
    inputFile = fopen(srcFileName, "r");
    dataImgTmp = fopen(dataTmpFileName, "r");/*created in phase one*/
    outPutOb = fopen(outPutFileName, "a"); /*created in phase one ill append the rest of data*/
    outPutExt = fopen(outPutExtFileName, "w");
    outPutEnt = fopen(outPutEntFileName, "w");



    if (inputFile == NULL || dataImgTmp == NULL || outPutOb == NULL || outPutExt == NULL || outPutEnt == NULL) {
        flags->dontMakeFile = TRUE;
        eFlags->unableToCrateFile = TRUE;
        goto FINISH;
    }

    while (fgets(line, MAX_LINE, inputFile) != NULL) {
        if( *(line+strlen(line)-1) != '\n' || strlen(line) >= MAX_LINE || isNoteLine(line) || isEmptyLine(line))
            goto SKIP;
        if (strstr(line, EXTERN_INSTRUCTION) == NULL
            && strstr(line, DATA_INSTRUCTION) == NULL
            && strstr(line, STRING_INSTRUCTION) == NULL
            && strstr(line, ENTRY_INSTRUCTION) == NULL
            ) {
            /* if the line parsed isn't empty nor note nor instruction
             * all of the above where defined in pahse one
             */

                splitter = (strchr(line, ':') != NULL) ? strchr(line, ':') + 1 : line;/*if there is a symbol skip it*/
                if (instructionSplitter(splitter, &instruction1) == LINE_PARSED) {
                    if ((result = crateOBline(outPutOb, &instruction1, *IC)) != OB_LINE_CREATED) {
                        flags->dontMakeFile = TRUE;
                        eFlags->addressingError = (result == ADRESSING_ERROR)?TRUE:FALSE;
                        eFlags->unknowOprand = (result == UNKNOW_OPRAND)?TRUE:FALSE;
                        eFlags->unknowInteger = (result == NON_INTEGER)?TRUE:FALSE;
                        if(result != UNKNOW_OPRAND && result != ADRESSING_ERROR && result != NON_INTEGER)
                            eFlags->unknowError = TRUE;
                    }
                    printToExternal(&instruction1,*IC,outPutExt);
                    *IC = *IC + sizeOfInstruction(&instruction1);
                    instruction1.cmd = 0;
                    instruction1.oprandsCount = 0;
                    instruction1.dis = NULL;
                    instruction1.src = NULL;
                }

        }
        SKIP:
        lineCounter++;
        printError(eFlags, flags, lineCounter);

    }
    insertDataToObj(outPutOb, dataImgTmp, *IC + START_INSTRUCTION_LOCATION);
    printEntryFile(outPutEnt, &EsymbolListRoot);
    FINISH:
    /*if the symbol list for ent or extern is empty so no need to crate file*/
    if (lenListWithParm(&EsymbolListRoot,entry) == 0) {
        remove(outPutEntFileName);
    }
    if (lenListWithParm(&EsymbolListRoot, external) == 0) {
        remove(outPutExtFileName);
    }

    if (flags->dontMakeFile || (*IC+START_INSTRUCTION_LOCATION) >= MAX_MEMORY_SIZE) {
        remove(outPutEntFileName);
        remove(outPutExtFileName);
        remove(outPutFileName);
    }

    remove(dataTmpFileName);
    if(outPutOb != NULL)fclose(outPutOb);
    if(dataImgTmp != NULL)fclose(dataImgTmp);
    if(outPutEnt != NULL)fclose(outPutEnt);
    if(outPutExt != NULL)fclose(outPutExt);


    printError(eFlags, flags, lineCounter);

    return !flags->dontMakeFile;


}
/**
 * while print the ent file
 * @param outPutFILE the fil to be printed to
 * @param root the linklist where all entry symbol are
 * @return
 */
BOOL printEntryFile(FILE *outPutFILE, PtSymbol root) {
    int lengthListWithParm1 = lenListWithParm(root, entry);
    PtSymbol lastOcurrence = &EsymbolListRoot;
    if (lengthListWithParm1 == 0) {
        return FALSE;
    }

    for (lastOcurrence = findNextSymbolWithParm(lastOcurrence, entry);
         lastOcurrence;
         lastOcurrence = findNextSymbolWithParm(lastOcurrence->next, entry)) {
        fprintf(outPutFILE, "%s %d\n", lastOcurrence->symbolName, lastOcurrence->location);
    }

    return TRUE;

}

void printToExternalFile(FILE *output, char *symbol, UINT IC) {
    UINT tIC = IC;
    int i = 3;
    fprintf(output, "%s ", symbol);

    while ((int) (tIC / pow(10, i)) == 0) {
        fprintf(output, "0");
        tIC = tIC % (int) pow(10, i--);
    }
    fprintf(output, "%d\n", IC);
}

void printToExternal(pInstruction instruction1, UINT IC, FILE* outPutExt)
{
    if (instruction1->src && isExtern(instruction1->src, &EsymbolListRoot)) {
        IC = IC + 1;
        printToExternalFile(outPutExt, instruction1->src, IC + START_INSTRUCTION_LOCATION);
    }
    if (instruction1->dis && isExtern(instruction1->dis, &EsymbolListRoot)) {
        if (instruction1->src != NULL && instruction1->oprandsCount == (UINT) 2) {
            IC = IC + 2;
        } else {
            IC = IC + 1;
        }

        printToExternalFile(outPutExt, instruction1->dis, IC + START_INSTRUCTION_LOCATION);
    }
}

/*TODO:
 * 1. test of symbol is leagel
 * 2. test if symbol is savedWord NEED TO BE TASTED!!!
 * 3. test if number in the input is in range
 * 4. string is ended with " or started
 * */