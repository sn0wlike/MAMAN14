
#include "object.h"



extern tSymbol symbolListRoot;
extern tSymbol EsymbolListRoot;


BOOL testForDestenationMethod(pInstruction ins,int disAddressType ,int srcAddressType);



unsigned int toNumber(char *oprand)
{
    oprand = strchr(oprand,'r')+1;
    return SHIFT_TO_NUMBER((*oprand));
}


/**
 * is Ascii is an Integer
 * @param number
 * @return
 */
BOOL isAanI(char* number)
{
    int x = atof(number);
    float y = atof(number);
    return  x == y;
}

int parseToRAW(char *op,int* adress,int*oprandARE)
{
    char* splitter;

    PtSymbol node = NULL;
    node = isSymbol(op,&symbolListRoot);
    if(node)
    {
        *adress = node->location;
        *oprandARE = RELATIVE_ARE;
    }else if( (node=isSymbol(op,&EsymbolListRoot)) != NULL )
    {
        *adress = 0;
        *oprandARE = EXTERNAL_ARE;
    }else if(strchr(op,'#') || strchr(op,'r'))
    {
        if((splitter = strchr(op,'#'))!=NULL) {
            if(!isAanI(splitter+1))
                return NON_INTEGER;

            if (*splitter == '-') {
                *adress = -1 * atoi(splitter + 1);
            } else {
                *adress = atoi(splitter+1);
            }
            if(*adress > MAX_NUMBER_VALUE_NOT_DATA || *adress < MIN_NUMBER_VALUE_NOT_DATA)
            {
                return OUT_OF_RANGE;
            }
        }else if((splitter = strchr(op,'r'))!=NULL)
        {
            *adress = SHIFT_TO_NUMBER(*(splitter+1));
        }
        *oprandARE = ABSOLUTE_ARE;
    }else
    {
        return FALSE;
    }
    return TRUE;

}


int crateOBline(FILE* f, pInstruction ins, UINT IC)
{
    BOOL doualRgisterOprand = FALSE;
    object obInstruct={0},obDis={0},obSrc={0};
    /*instruction definition*/
    int opCode = 0;
    UCHAR disAddressType = 0;
    UCHAR srcAddressType = 0;
    int insARE = 4;/*define as 4 constant for instracion ARE*/
    /*oprands definition*/
    int adress = 0;
    int oprandARE = 0;

    /*Parsing Ins*/
    opCode = ins->cmd;


    if(ins->oprandsCount == 2) {

        adreesMathodidentifier(&disAddressType, ins->dis);
        adreesMathodidentifier(&srcAddressType, ins->src);
        /*if both mehthods are on register we should crate one line if code for both*/

        if((disAddressType == DIRECT_REGISTER_PASS || disAddressType == NON_IMMEDIATE_REGISTER_PASS )
           &&
           (srcAddressType == DIRECT_REGISTER_PASS|| srcAddressType == NON_IMMEDIATE_REGISTER_PASS))
        {
            doualRgisterOprand = TRUE;

        }
    }else if(ins->oprandsCount == 1)
    {
        adreesMathodidentifier(&disAddressType, ins->dis);
        srcAddressType = 0;
    }else {
        disAddressType = 0;
        srcAddressType = 0;
    }

    obInstruct.ob = 0;
    obInstruct.ob += opCode * SHIFT_BY(INSTRACTION_OPCODE_BIT_LOCATION);
    obInstruct.ob += srcAddressType * SHIFT_BY(INSTRACTION_ADDRESS_SRC_METHOD);
    obInstruct.ob += disAddressType * SHIFT_BY(INSTRACTION_ADDRESS_DIS_METHOD);
    obInstruct.ob += insARE * SHIFT_BY(INSTRACTION_OPCODE_ARE_LOCATION);

    if(doualRgisterOprand)
    {
        adress += toNumber(ins->dis);
        adress += toNumber(ins->src)*SHIFT_BY(3);
        oprandARE = 4;
        obDis.ob = 0;
        obDis.ob +=oprandARE *SHIFT_BY(INSTRACTION_OPCODE_ARE_LOCATION);
        obDis.ob +=adress *SHIFT_BY(OPRAND_ADDRESS_LOCATION);
    }else
    {
        if(ins->oprandsCount == 2)
        {

            if(parseToRAW(ins->src,&adress,&oprandARE)) {
                obSrc.ob = 0;
                obSrc.ob += oprandARE * SHIFT_BY(INSTRACTION_OPCODE_ARE_LOCATION);
                if(srcAddressType == NON_IMMEDIATE_REGISTER_PASS || srcAddressType == DIRECT_REGISTER_PASS)
                {
                    adress = adress*SHIFT_BY(3);
                    obSrc.ob += adress * SHIFT_BY(OPRAND_ADDRESS_LOCATION);
                }else{
                    obSrc.ob += adress * SHIFT_BY(OPRAND_ADDRESS_LOCATION);
                }
            }else if(parseToRAW(ins->dis,&adress,&oprandARE) == NON_INTEGER)
            {
                return NON_INTEGER;
            }
            else
            {
                return UNKNOW_OPRAND;
            }
            if(parseToRAW(ins->dis,&adress,&oprandARE) == TRUE) {
                obDis.ob = 0;
                obDis.ob += oprandARE * SHIFT_BY(INSTRACTION_OPCODE_ARE_LOCATION);
                obDis.ob += adress * SHIFT_BY(OPRAND_ADDRESS_LOCATION);
            }else if(parseToRAW(ins->dis,&adress,&oprandARE) == NON_INTEGER)
            {
                return NON_INTEGER;
            }
            else
            {
                return UNKNOW_OPRAND;
            }


        }else if( ins->oprandsCount == 1)
        {
            if(parseToRAW(ins->dis,&adress,&oprandARE) == TRUE) {
                obDis.ob = 0;
                obDis.ob += oprandARE * SHIFT_BY(INSTRACTION_OPCODE_ARE_LOCATION);
                obDis.ob += adress * SHIFT_BY(OPRAND_ADDRESS_LOCATION);
            }else if(parseToRAW(ins->dis,&adress,&oprandARE) == NON_INTEGER)
            {
                return NON_INTEGER;
            }
            else
            {
                return UNKNOW_OPRAND;
            }
            obSrc.ob = 0;
        }else {

            obDis.ob = 0;
            obSrc.ob = 0;
        }
    }
    if(testForDestenationMethod(ins,disAddressType,srcAddressType) == FALSE) {
        return ADRESSING_ERROR;
    }


    if(obInstruct.ob != 0) {
        fprintf(f,"%d ",IC+START_INSTRUCTION_LOCATION);
        oct(f,obInstruct.ob);
        fprintf(f,"\n");
        IC++;
    }
    if(obSrc.ob != 0) {
        fprintf(f,"%d ",IC+START_INSTRUCTION_LOCATION);
        oct(f,obSrc.ob);
        fprintf(f,"\n");
        IC++;
    }
    if(obDis.ob != 0) {
        fprintf(f,"%d ",IC+START_INSTRUCTION_LOCATION);
        oct(f,obDis.ob);
        fprintf(f,"\n");
        IC++;
    }
    return OB_LINE_CREATED;


}

BOOL testForDestenationMethod(pInstruction ins,int disAddressType ,int srcAddressType)
{
    int cmd = ins->cmd;
    /*Testing source part of table in page 30*/
    if(cmd >= 0 && cmd<=3)
    {
        if(srcAddressType == 0)
            return FALSE;
    }
    if(cmd == 4)
    {
        if(srcAddressType != DIRECT_OPRAND_PASS)
            return FALSE;
    }
    if(cmd >=5 && cmd <= 15)
    {
        if(srcAddressType != 0)
            return FALSE;
    }
    /*Testing dis part of table in page 30*/
    if(cmd == 0)
    {
        if(disAddressType == DIRECT_REGISTER_PASS || disAddressType == 0)
            return FALSE;
    }
    if(cmd == 1 || cmd == 12)
    {
        if(disAddressType == 0)
            return FALSE;
    }
    if((cmd >= 2 && cmd <=8) || cmd == 11 )
    {
        if(disAddressType == 0 || disAddressType == IMMEDIATE_OPRAND_PASS)
        {
            return FALSE;
        }
    }
    if((cmd>=9 && cmd<=10) || cmd == 13)
    {
        if(disAddressType != IMMEDIATE_OPRAND_PASS && disAddressType != DIRECT_OPRAND_PASS)
            return FALSE;
    }
    if(cmd >= 14 && cmd<=15)
    {
        if(disAddressType != 0)
            return FALSE;
    }
    return TRUE;
}

int adreesMathodidentifier(UCHAR *methode,char* oprand)
{
    if(*oprand == 'r')
    {
        if(strlen(oprand) == 2)
        {
            if(*(oprand+1)>=0x30 && *(oprand+1)<=0x37 )
            {
                *methode =DIRECT_REGISTER_PASS;
                return 1;
            }
        }
    }else {
        if ((*oprand) == '#') {
            *methode = IMMEDIATE_OPRAND_PASS;
            return 1;
        } else if ((*oprand) =='*') {
            *methode =NON_IMMEDIATE_REGISTER_PASS;
            return 1;
        }
    }

    {
        *methode =DIRECT_OPRAND_PASS;
        return 1;
    }
}

