//
// Created by Kev on 25/01/2020.
//
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "util.h"
#include "structures.h"
#include "processing.h"


int printer(IS* root,unsigned int sN){
    int i=0;
    for(i;i<sN;i++){

        fprintf(output_r,"\n\n\n");
        if(VERBOSE==0){
            fprintf(output_r,"%d  ",max_id);
            decoding(root->sons[i]->encoded_sequence,output_r);
            //fprintf(output_r,"%d  <%s>  ",max_id,root->sons[i]->sequence);//TODO SISTEMARE LA PARTE ENCODED
            max_id++;
        }
        else if(VERBOSE==1){
            fprintf(output_r,"%d  ",max_id);
            decoding(root->sons[i]->encoded_sequence,output_r);
            fprintf(output_r,"rsup: %.3f  ",root->sons[i]->rsup);
            max_id++;
        }
        else if(VERBOSE==2){
            fprintf(output_r,"%d  ",max_id);
            decoding(root->sons[i]->encoded_sequence,output_r);
            fprintf(output_r,"support: %d  ",root->sons[i]->support);
            fprintf(output_r,"rsup: %.3f  ",root->sons[i]->rsup);
            max_id++;
        }

        if(tdlists==2) {
            fprintf(output_r,"\nV-idlist\n");
            line *start = root->sons[i]->table->head;
            if (start == NULL) return 0;

            while (start->next != NULL) {
                fprintf(output_r,"sid: %u, eid: %u \n", start->sid, start->eid);
                start = start->next;
            }
            fprintf(output_r,"sid: %u, eid: %u \n", start->sid, start->eid);
        }
        else if(tdlists==1) {
            unsigned int xsid;
            fprintf(output_r,"sid: ");
            line *start = root->sons[i]->table->head;
            if (start == NULL) return 0;
            fprintf(output_r,"{");
            ///////////////////
            xsid=start->sid;
            fprintf(output_r," %u ", start->sid);
            start = start->next;
            ///////////////////
            while (start!= NULL) {
                if(start->sid!=xsid) {
                    fprintf(output_r, " %u ", start->sid);
                    xsid=start->sid;
                }
                start = start->next;
            }
            fprintf(output_r,"}");
        }

    }
    return 0;
}




//TODO if tooo slow we can save the tail and just pass the tail address for the add
int addLine(IS* atom, int sid,int eid){//costant
    line* lin = atom->table->tail;

    if(lin==NULL){//first time head=tail
        atom->table->head=(line*) calloc(1, sizeof(line));
        atom->table->tail = atom->table->head;
        atom->table->tail->sid=sid;
        atom->table->tail->eid=eid;
        atom->support++;
        return 0;
    }
    if(lin->sid!=sid){
        atom->support++;
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
        atom->table->tail=lin->next;
        atom->table->tail->next=NULL;
    }
    else if(lin->eid!=eid){//don't add the same lines all the time, since they are in order the last one is the biggest
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
        atom->table->tail=lin->next;
        atom->table->tail->next=NULL;
    }
    return 0;


/* VERSION OF THE CODE WHERE I SCAN THE WHOLE TABLE TO ADD A NEW LINE
    line* lin = atom->table->head;

    if(lin==NULL){
        atom->table->head= (line*) calloc(1, sizeof(line));
        atom->table->head->sid=sid;
        atom->table->head->eid=eid;
        atom->support++;
        return 0;
    }

    while(lin->next!=NULL){
        lin = lin->next;
    }

    if(lin->sid!=sid){
        atom->support++;
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
    }
    else if(lin->eid!=eid){//don't add the same lines all the tieme, since they are in order the last one is the biggest
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
    }
    return o;
*/
}






//TODO DECIDE IF I NEED HASHING
IS* present(char* item, IS* root, unsigned int sonNumb){
    int i=0;
    //int r;
    IS* RESULT=NULL;
    for(i; i<sonNumb;i++){
        if(strcmp(root->sons[i]->sequence,item)==0) {//r=
            RESULT=root->sons[i];
            return root->sons[i];
        }
    }
    return NULL;
}
IS* present_memcmp(unsigned int* item, IS* root, unsigned int sonNumb){
    int i=0;
    size_t lenx=lenarray(item);
    //int r;
    IS* RESULT=NULL;
    for(i; i<sonNumb;i++){
        if (memcmp(root->sons[i]->encoded_sequence, item,lenx) == 0) {//r=
            RESULT = root->sons[i];
            return root->sons[i];
        }
    }
    return NULL;
}

