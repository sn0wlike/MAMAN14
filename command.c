
#include "command.h"


/**
 * @brief this funvtion returnn the Opcode of the the instruction
 * @param oprand string of the instruction
 * @param howManyOprand this will be set with the proper oprand amount for instruttion
 * @return
 */
char convertToOpcode(char* oprand,char* howManyOprand)
{
    char i = hasInstruction(oprand);

            if( i >= 0 && i <=4)
            {
                *howManyOprand = 2;
            }else if(i>=5 && i<=13)
            {
                *howManyOprand = 1;
            }else
            {
                *howManyOprand = 0;
            }
            return i;
}

/**
 * @brief this function returns if the giving text has an instruction
 * @param line the txt to be chhecked
 * @return return -1 if noFound or non negative number
 */
char hasInstruction(char *line) {
    short i = 0;
    char* commands[OPRAND_AMOUNT] = OPRAND_LIST;


    for(i=0;i<OPRAND_AMOUNT;i++)
    {
        if(strstr(line,commands[i]) != 0) {
            return i;
        }
    }
    return NO_COMMAND_FOUND;

}

/**
 * @brief cheaks if given text has register inside
 * @param dis text to be tested
 * @return return TRUE or FALSE
 */
BOOL isRegister(char* dis)
{
    if(dis == NULL)
    {
        return FALSE;
    }
    if(strlen(dis) >= 2 && strlen(dis) <=3)
    {
        if((dis = strchr(dis,'r')) != NULL)
        {
            if(*(dis+1) >= 0x30 && *(dis+1)<=0x37)
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**
 * @brief this will parse from given text to instruction struct
 * @param line text to be parsed
 * @param Command distention struct
 * @return will return status of successes
 */
int instructionSplitter(char *line, pInstruction Command) {

    char* split = NULL;
    char* current  = (strchr(line,':') != NULL)?strchr(line,':')+1:line;
    char operands = 0;
    int i =0;
    char* commands[OPRAND_AMOUNT] = OPRAND_LIST;

    while ( i<OPRAND_AMOUNT && strstr(current,commands[i++]) == NULL);
    if(i > OPRAND_AMOUNT)
    {
        return NO_COMMAND_FOUND;
    } else{
        current = strstr(current,commands[i-1]);
    }
    if(strchr(current,'\n') || strchr(current,' '))
    {
        split = ( strchr(current,'\n')>strchr(current,' ') &&
                strchr(current,' ')!=NULL)?strchr(current,' '):strchr(current,'\n');
        *split = '\0';
        Command->cmd =convertToOpcode(current,&operands);
        Command->oprandsCount = operands;
        *split = ( strchr(current,'\n')>strchr(current,' ') &&
                   strchr(current,' ')!=NULL)?' ':'\n';
        current = split+1;
        clrSpaces(current);

        if(operands == 0)
        {
            Command->dis = NULL;
            Command->src = NULL;
            return LINE_PARSED;
        }else if(operands == 1)
        {
            Command->src = NULL;
            Command->dis = current;
            return LINE_PARSED;
        }else if (operands == 2){

            if((split = strchr(current,',')) != NULL){
                *split = '\0';
                Command->src = current;
                Command->dis = split+1;

                return LINE_PARSED;
            } else{
                return MISSING_COMA;
            }
        }

    }
    return 0;
}

/**
 * @brief return the size of given instruction in
 * @param instruction1 instruction to be sized
 * @return size of the instruction
 */
int sizeOfInstruction(pInstruction instruction1) {

    int i = 0;
    if(instruction1->cmd >= OPRAND_MIN_VALUE && instruction1->cmd <= OPRAND_MAX_VALUE)
    {
        i++;
    }
    if(instruction1->dis == NULL) {
        return i;
    }
    if(isRegister(instruction1->dis) && isRegister(instruction1->src))
    {
            i++;
    } else{
        i += instruction1->oprandsCount;
    }
    return i;
}
