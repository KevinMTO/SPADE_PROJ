//
// Created by Kev on 25/01/2020.
//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "structures.h"
#include "pseudo_pruning.h"


void cleanTable(TID* tab){
    line* l;
    l =tab->head;
    line *temp = NULL;
    if (l != NULL) {
        while (l->next != NULL) {
            temp = l;
            l = l->next;
            free(temp);
        }

        free(l);
    }
}
void clean(IS* node) {
    char *who = node->sequence;// FOR DEBUGGING
    if(node->table!=NULL) {
        cleanTable(node->table);
        free(node->sequence);
        free(node->table);
    }
}

unsigned int updateRelSup(IS* root, unsigned int sonNumb, float min_sup, unsigned int tot){//works with encoded sequnces
    int i=0;
    int realSons=sonNumb;
    int p,counter;
    for(i; i<sonNumb;i++){
        root->sons[i]->rsup=(float) root->sons[i]->support/tot;
        //printf("DEBUG FLOAT %s %f",root->sequence,root->sons[i]->rsup);
        //compute rsup


        p=0,counter=tot; //checks sequence hasn't more transactions than sids
        while(root->sons[i]->encoded_sequence[p]!=0){
            if(root->sons[i]->encoded_sequence[p]==3) counter--;
            p++;
        }

        if(root->sons[i]->rsup<min_sup || counter<0){
            clean(root->sons[i]);
            root->sons[i]=NULL;
            realSons--;
        }
    }
    if(realSons!=sonNumb) {
        IS **new_sons = (IS **) calloc(realSons, sizeof(IS *));
        int k = 0;
        for (i = 0; i < realSons; i++) {
            while (k < sonNumb) {
                if (root->sons[k] != NULL) {
                    new_sons[i] = root->sons[k];
                    k++;
                    break;
                }
                k++;
            }
        }
        free(root->sons);
        root->sons = new_sons;
    }
    return realSons;
}
