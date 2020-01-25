//
// Created by Kev on 25/01/2020.
//

#ifndef SPADE_PROJ_PSEUDO_PRUNING_H
#define SPADE_PROJ_PSEUDO_PRUNING_H


void cleanTable(TID* tab);

void clean(IS* node);

unsigned int updateRelSup(IS* root, unsigned int sonNumb, float min_sup, unsigned int tot);

#endif //SPADE_PROJ_PSEUDO_PRUNING_H
