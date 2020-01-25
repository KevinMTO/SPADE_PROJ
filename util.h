//
// Created by Kev on 25/01/2020.
//

#ifndef SPADE_PROJ_UTIL_H
#define SPADE_PROJ_UTIL_H

#include "structures.h"

int printer(IS* root, unsigned int sN);


int addLine(IS* atom, int sid,int eid);

IS* present(char* item, IS* root, unsigned int sonNumb);

IS* present_memcmp(unsigned int* item, IS* root, unsigned int sonNumb);
#endif //SPADE_PROJ_UTIL_H
