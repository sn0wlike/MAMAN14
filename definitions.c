

#include "definitions.h"



BOOL printError(pE_Flag eFlag, pFlag flag1, int lineCounter) {
    BOOL retVal = FALSE;
    if (eFlag->unknownInstrction) {
        printf("ERROR:unknown Instrction inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->symbolExsitis) {
        printf("ERROR:symbol exsitis inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->errorInTheData) {
        printf("ERROR:data givin is not in range inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->unknowOprand) {
        printf("ERROR:oprand is unknown inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->unknowError) {
        printf("ERROR:unknown error inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->addressingError) {
        printf("ERROR:incorrect addressing method inline: %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->undefeindedEntrySymbol) {
        printf("ERROR: undfeined entry point inline %d\n", lineCounter);
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->unableToCrateFile) {
        printf("ERROR: while opening or creating output files");
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    if (eFlag->tooLongLine) {
        printf("ERROR: Line too long");
        flag1->dontMakeFile = TRUE;
        retVal = TRUE;
    }
    eFlag->unknownInstrction = FALSE;
    eFlag->symbolExsitis = FALSE;
    eFlag->errorInTheData = FALSE;
    eFlag->addressingError = FALSE;
    eFlag->unknowError = FALSE;
    eFlag->unknowOprand = FALSE;
    eFlag->undefeindedEntrySymbol = FALSE;
    eFlag->unableToCrateFile = FALSE;
    eFlag->tooLongLine = FALSE;
    return retVal;
}
