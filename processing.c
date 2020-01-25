//
// Created by Kev on 25/01/2020.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "structures.h"
#include "processing.h"

unsigned int * encode(unsigned int val1,unsigned int val2, unsigned int* prefix, unsigned int prefix_length, int type, int equality, int prefix1, int prefix2){
    //0='\0', 1=',', 2='{', 3='}'

    //atom Y=0, prefix=NULL, prefix length =0, type=0
    if(val1 != 0 && val2 == 0 && prefix == NULL && prefix_length == 0 && type == 1){
        unsigned int *encoding = (unsigned int *) calloc(4, sizeof(unsigned int));
        encoding[0] = 2;
        encoding[1] = val1;
        encoding[2] = 3;
        encoding[3] = 0;
        return encoding;
    }
    else if(val1 != 0 && val2 != 0 && prefix == NULL && prefix_length == 0 && type == 2){//two_sequence type 2
        if(equality==0) {
            unsigned int *encoding = (unsigned int *) calloc(8, sizeof(unsigned int));
            encoding[0] = 2;
            encoding[1] = val1;
            encoding[2] = 3;
            encoding[3] = 1;
            encoding[4] = 2;
            encoding[5] = val2;
            encoding[6] = 3;
            encoding[7] = 0;
            return encoding;
        }
        else{
            unsigned int *encoding = (unsigned int *) calloc(6, sizeof(unsigned int));
            encoding[0] = 2;
            encoding[1] = val1;
            encoding[2] = 1;
            encoding[3] = val2;
            encoding[4] = 3;
            encoding[5] = 0;
            return encoding;
        }
    }
    else if(val1 != 0 && val2 != 0 && prefix != NULL && prefix_length != 0 && type == 3){
        if(equality==0){
            if(prefix1==0 && prefix2==0){

                unsigned int *encoding = (unsigned int *) calloc((prefix_length+9), sizeof(unsigned int));

                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+2]=val1;
                encoding[pos+3]=3;
                encoding[pos+4]=1;
                encoding[pos+5]=2;
                encoding[pos+6]=val2;
                encoding[pos+7]=3;
                encoding[pos+8]=0;
                return encoding;
            }
            else if(prefix1==1 && prefix2==0){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+5), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+2]=val2;
                encoding[pos+3]=3;
                encoding[pos+4]=0;
                return encoding;
            }

        }
        else if(equality==1){
            if(prefix1==0 && prefix2==0){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+7), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+2]=val1;
                encoding[pos+3]=1;
                encoding[pos+4]=val2;
                encoding[pos+5]=3;
                encoding[pos+6]=0;
                return encoding;
            }
            else if(prefix1==1 && prefix2==1){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+6), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=val1;
                encoding[pos+2]=1;
                encoding[pos+3]=val2;
                encoding[pos+4]=3;
                encoding[pos+5]=0;
                return encoding;
            }
        }
        else if(equality==2){
            if(prefix1==0 && prefix2==1){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+5), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+2]=val1;
                encoding[pos+3]=3;
                encoding[pos+4]=0;
                return encoding;
            }
            else if(prefix1==0 && prefix2==0){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+9), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+2]=val2;
                encoding[pos+3]=3;
                encoding[pos+4]=1;
                encoding[pos+5]=2;
                encoding[pos+6]=val1;
                encoding[pos+7]=3;
                encoding[pos+8]=0;
                return encoding;
            }
        }
    }


}
unsigned int lenarray(unsigned int * array){
    unsigned int i=0;
    while(array[i]!=0) i++;
    return i;
}
unsigned int numEXTRACTOR(unsigned int* array){// numEXTRACTOR is built based on the assumptions and the conventions adopted for the numerical representation
    unsigned int j=0;
    for(j;j<lenarray(array);j++){
        if(array[j]!=1 && array[j]!=2 && array[j]!=3){
            return array[j];
        }
    }
}

void decoding(unsigned int * encoding, FILE* WHERE){
    unsigned int lenny= lenarray(encoding);
    fprintf(WHERE,"<");
    unsigned int i=0;
    for(i;i<lenny;i++){
        switch(encoding[i]){
            case 0: fprintf(WHERE,"%c",'\0');
                break;
            case 1: fprintf(WHERE,"%c",',');
                break;
            case 2:fprintf(WHERE,"%c",'{');
                break;
            case 3:fprintf(WHERE,"%c",'}');
                break;
            default:fprintf(WHERE,"%s",ROOT->sons[(encoding[i]-4)]->sequence);
                break;
        }
    }
    fprintf(WHERE,">");
    fprintf(WHERE,"   ");
}
