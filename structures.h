//
// Created by Kev on 25/01/2020.
//

#ifndef SPADE_PROJ_STRUCTURES_H
#define SPADE_PROJ_STRUCTURES_H

//TODO IDEA HASHING W. tail
struct lineInTable{
    unsigned int sid;
    unsigned int eid;
    struct lineInTable* next;
}typedef line;

struct tidlist{
    line * head;
    line * tail;
}typedef TID;

struct sequence{
    unsigned int id;
    int prefix;
    char * sequence;
    unsigned int* encoded_sequence;

    unsigned int support;
    float rsup;

    TID * table;
    unsigned int nSons;
    struct sequence** sons;
}typedef IS;

struct freq1{
    IS* root;
    unsigned int atoms;
    unsigned int tot_sids;
}typedef F1;

//GLOBAL VARIABLE IN THE WALL OF CODE
extern FILE* output_r;
extern unsigned int id_counter;
//int BFS=0;
//int DFS=1;
extern int VERBOSE;
extern int tdlists;
extern unsigned int max_id;
extern IS* ROOT;

#endif //SPADE_PROJ_STRUCTURES_H
