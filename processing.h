//
// Created by Kev on 25/01/2020.
//

#ifndef SPADE_PROJ_PROCESSING_H
#define SPADE_PROJ_PROCESSING_H

unsigned int * encode(unsigned int val1,unsigned int val2, unsigned int* prefix, unsigned int prefix_length, int type, int equality, int prefix1, int prefix2);
unsigned int lenarray(unsigned int * array);

unsigned int numEXTRACTOR(unsigned int* array);

void decoding(unsigned int * encoding, FILE* WHERE);


#endif //SPADE_PROJ_PROCESSING_H
