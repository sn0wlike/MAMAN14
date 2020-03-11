
#include "io_maneger.h"


char* Realloc(char *ptr,size_t size,size_t src_size);

char* readLine(FILE* fp, size_t size) {
    /*The size is extended by the input with the value of the provisional*/
    char *str = malloc(size);/*The string pointer that ill use*/
    int ch;
    size_t len = 0;
    str = Realloc(NULL, sizeof(char) * size,size);/*size is start size*/
    if (!str)return str; /*if the realloc was not able to allocate the memory*/
    while (EOF != (ch = fgetc(fp)) && ch != '\n' && ch != '\0') {
        str[len++] = ch;/*adds to the string the input*/
        if (len == size) { /*if needed more memory  I'll realloc more */
            str = Realloc(str, sizeof(char) * (size + size),size);
            size = size*2;
            if (!str)return str;/*if the realloc was not able to allocate the memory*/
        }
    }
    /*here ill add the '\0' to end string but first test if there is room for the '\0'*/
    if (len+1 == size) { /*if needed more memory  I'll realloc more */
        str = Realloc(str, sizeof(char) * (size + 1),size);
        size = size*2;
        if (!str)return str;/*if the realloc was not able to allocate the memory*/
    }
    str[len++] = '\0';/*NULL teraminating the string*/
    if(ch == EOF)
    {
        return 0;
    }
    return str;
}


char* Realloc(char *ptr,size_t size,size_t src_size)
{
    char *p = malloc(size);
    if(ptr != NULL) {
        memcpy(p,ptr, src_size);
        return p;
    }
    if(p != NULL) {
        free(ptr);
        return p;
    }
    return NULL;
}


void clrSpaces(char* s)
{
    int writer = 0, reader = 0;
    char spaces [] = SPACES_ARRAY;
    int i = 0;
    for(i = 0;i<3;i++) {
        writer = 0;
        reader = 0;
        while (s[reader]) {
            if (s[reader] != spaces[i]) {
                s[writer++] = s[reader];
            }
            reader++;
        }
        s[writer] = 0;
    }
}

BOOL printToDataFile(char *line, FILE *f, UINT *DC, pE_Flag errorFlag) {

    char *dataS = line,*dataE=NULL,*split=NULL,data=0;
    int i =1;
    unsigned  int x = MSB_ON;
    int tmp = 0;
    if(strstr(line,"string")) {
        dataS = strchr(dataS, '"')+1;
        dataE = strchr(dataS, '"');
        if(dataE == NULL) {
            errorFlag->noStringEnd = TRUE;
            return NO_STRING_TERMANIATOR;
        }
        while(dataS != dataE)
        {
            while(i<=15)
            {
                tmp = (((*dataS)&x) == 0 )?0:1;
                fprintf(f,"%d",tmp);
                x = x/2;
                i++;
            }
            i=1;
            x= MSB_ON;
            fprintf(f,"\n");
            dataS++;
            (*DC)++;
        }
        /*print the NULL traimnted fanilizer*/
        fprintf(f,NULL_DALIMTER_STRING);
        (*DC)++;
    }else if(strstr(line,"data"))
    {
        dataS = strstr(dataS, ".data ")+strlen("data ");
        dataE = strchr(dataS, '\0');
        clrSpaces(dataS);
        while(dataS != NULL && dataS != dataE  )
        {
            split = strchr(dataS,',');
            if(split!=NULL)*split = '\0';
            if(atoi(((*dataS == '-')?dataS+1:dataS))<=CHAR_MAX
                && atoi(((*dataS == '-')?dataS+1:dataS)) >= CHAR_MIN){
                data = atoi(((*dataS == '-')?dataS+1:dataS));
                data = (*dataS == '-')?data*-1:data;
                while(i<=15)
                {
                    tmp = ((data&x) == 0 )?0:1;
                    fprintf(f,"%d",tmp);
                    x = x/2;
                    i++;
                }
                i=1;
                x= MSB_ON;
                fprintf(f,"\n");
                dataS++;
                (*DC)++;
            }else{
                printf("Error value out of range");
                return FALSE;
            }
            if(split!=NULL)*split=',';
            dataS=(split!=NULL)?split+1:NULL;
        }
    }

    return TRUE;
}

void insertDataToObj(FILE *fObj, FILE *fData, UINT IC){
    char data[DATA_SIZE_ARRAY];
    int i = 0;
    unsigned long binnum = 0;
    unsigned int num = 0;
    int rem;
    while(fgets(data,DATA_SIZE_ARRAY,fData))
    {
        if(*data)
        {
            fprintf(fObj,"%u ",IC);
        }
        sscanf(data,"%lu",&binnum);

        while( binnum )
        {
            rem = binnum%2;
            num += rem * pow(2,i++);
            binnum = binnum/10;
        }
        if(*data != '\n') {
            IC++;
            oct(fObj, num);
            fprintf(fObj,"\n");
        }
        i=0;
        num =0;
    }



}


void oct(FILE*f,unsigned n)
{
    int i =0;
    int delta = 0;
    char txt[6] = {0};
    sprintf(txt,"%o",n);
    delta =5-strlen(txt);
    for(i =0;i<delta;i++)
    {
        fprintf(f,"0");
    }
    fprintf(f,"%o",n);
}


BOOL isEmptyLine(char *Line){
    char *temp = malloc(strlen(Line));
    memcpy(temp,Line,strlen(Line));
    clrSpaces(temp);
    if(strlen(Line) < 1 || *Line == '\n')
    {
        free(temp);
        return TRUE;
    }else {
        free(temp);
        return FALSE;
    }
}
BOOL isNoteLine(char *Lable)
{
    if(strchr(Lable,';') != NULL)
    {
        return TRUE;
    }else {
        return FALSE;
    }
}
/**
 * create the file name of files
 * @param nameFile ditanation where to save the new name file
 * @param inputFileName  source name file where we get the start of name
 * @param extension extantion to be added to the file name
 */
void makeFileName(char *nameFile, char *inputFileName, char *extension) {

    /*
     * File Name is the src file name with diffrent extension
     * algorithem:
     * 1.copy name
     * 2.look for .as
     * 3.replace to correct extension
     * */
    UINT sizeofstr= strlen(nameFile);
    strcpy((nameFile+sizeofstr), inputFileName);
    strcpy(strstr(nameFile, FILE_EXTENSION_INPUT), extension);

}

BOOL fileIsLegit(char *filename) {
    char *tmp;
    tmp = strstr(filename,FILE_EXTENSION_INPUT);
    if(tmp)
    {
        return (*(tmp+ strlen(FILE_EXTENSION_INPUT))=='\0')?TRUE:FALSE;

    } else
    {
        return FALSE;
    }

}
