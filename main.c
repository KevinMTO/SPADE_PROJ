#include<stdio.h>
#include<string.h>
#include<stdlib.h>


unsigned int empty_supp=0;

struct lineInTable{
    unsigned int sid;
    unsigned int eid;
    struct lineInTable* next;
}typedef line;

struct tidlist{
    line * tableline;
}typedef TID;

struct sequence{
    unsigned int id;
    char * prefix;
    char * sequence;

    unsigned int support;
    float rsup;

    TID * table;
    struct sequence** sons;
}typedef IS;


unsigned int updateRelSup(IS *pSequence,unsigned int sonNumb,float min_sup,unsigned int tot);

/////////////////////////////////////////////////////////////////////////////////////
int printer(IS* root,unsigned int sN){
    int i=0;
    for(i;i<sN;i++){
        printf("=============================================\n");
        printf("SEQUENCE: %s\n=================\n",root->sons[i]->sequence);
        printf("support: %d\n rsup: %f\n-----------\n",root->sons[i]->support,root->sons[i]->rsup);
        printf("V-idlist\n");
        line* start= root->sons[i]->table->tableline;
        if(start==NULL) return 0;

        while(start->next!=NULL){
            printf("sid: %u, eid: %u \n",start->sid,start->eid);
            start=start->next;
        }
        printf("sid: %u, eid: %u \n",start->sid,start->eid);
    }
    return 0;
}

//TODO if tooo slow we can save the tail and just pass the tail address for the add
int addLine(IS* atom, int sid,int eid){

    line* lin = atom->table->tableline;

    if(lin==NULL){
        atom->table->tableline= (line*) calloc(1, sizeof(line));
        atom->table->tableline->sid=sid;
        atom->table->tableline->eid=eid;
        atom->support++;
        return 0;
    }

    while(lin->next!=NULL){
        lin = lin->next;
    }

    if(lin->sid!=sid){
        atom->support++;
    }
    lin->next= (line*) calloc(1, sizeof(line));
    lin->next->sid=sid;
    lin->next->eid=eid;
    return 0;
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

IS* one_seq(FILE* fp, float min_sup){
    int initial_sons=6;
    unsigned int oldsid;
    unsigned int sonNumb=0;
    int id_counter=0;

    IS* empty_set= (IS*) calloc(1, sizeof(IS));
    empty_set->id=id_counter;
    id_counter++;
    empty_set->prefix=NULL;
    empty_set->sequence=NULL;
    empty_set->support=0;
    empty_set->sons= (IS**) realloc(NULL, initial_sons*sizeof(IS*));

    //printf(("here"));
    char * line = NULL;
    size_t len = 100;
    //ssize_t read;

    while ((getline(&line, &len, fp)) != -1) { //todo cancellare le azioni verso read var
        char * field1= strtok(line,"{ } \" ");
        //char * copy_field1= (char*) calloc(strlen(field1), sizeof(char));
        //strcpy(copy_field1,field1);

//        printf("field1 is %s\n", field1);
//        printf("Retrieved line of length %zu:\n", read);
//        printf("%s\n", field1);
        int sid = atoi(strtok(NULL,"{ }, \"  "));

        if(empty_set->support==0){
            oldsid=sid;
            empty_set->support++;
        }
        else if(sid!=oldsid){
            oldsid=sid;
            empty_set->support++;
        }

        int eid = atoi(strtok(NULL,"{ }, \"  "));

        //printf("sid %d, eid %d \n", sid,eid);
        char * decompose= strtok(field1,", ");

        while(decompose!=NULL){
            IS* atom=present(decompose,empty_set,sonNumb);

            if(atom==NULL){
                    //case we adding a new element atom
                    if(sonNumb>=initial_sons){
                        initial_sons= initial_sons*2;
                        empty_set->sons= (IS**) realloc(empty_set->sons, initial_sons*sizeof(IS*));
                    }

                    empty_set->sons[sonNumb] = (IS*) calloc(1, sizeof(IS));
                    empty_set->sons[sonNumb]->sequence=(char*) calloc(strlen(decompose), sizeof(char));
                    strcpy(empty_set->sons[sonNumb]->sequence,decompose);
                    empty_set->sons[sonNumb]->prefix=(char*) calloc(strlen(decompose), sizeof(char));
                    strcpy(empty_set->sons[sonNumb]->prefix,decompose);

                    empty_set->sons[sonNumb]->id=id_counter;
                    id_counter++;

                    empty_set->sons[sonNumb]->sons=NULL;
                    empty_set->sons[sonNumb]->table=(TID*) calloc(1,sizeof(TID));
                    //of sid is new allocate new spaces and append

                    addLine(empty_set->sons[sonNumb],sid,eid);
                    //compute rsup


                    sonNumb++;
            }
            else{
                    //based on asssumption that we're reading data already order
                    addLine(atom,sid,eid);
                    //compute rsup
                    //empty_set->sons[sonNumb]->rsup=((float)empty_set->sons[sonNumb]->support)/((float)empty_set->support);
            }

            decompose= strtok(NULL,", ");

        }


    }
    unsigned int realsons=updateRelSup(empty_set,sonNumb,min_sup,empty_set->support);
    printer(empty_set,realsons);
return NULL;
}

unsigned int updateRelSup(IS* root, unsigned int sonNumb, float min_sup, unsigned int tot){
    int i=0;
    int realSons=sonNumb;
    line* l;
    for(i; i<sonNumb;i++){
        root->sons[i]->rsup=(float) root->sons[i]->support/tot;
        //compute rsup
        if(root->sons[i]->rsup<min_sup){
            char* who= root->sons[i]->sequence;
            free(root->sons[i]->sequence);
            free(root->sons[i]->prefix);
            l=root->sons[i]->table->tableline;
            line*temp=NULL;
            while(l->next!=NULL){
                temp=l;
                l = l->next;
                free(temp);
            }

            free(l);
            free(root->sons[i]->table);
            root->sons[i]=NULL;
            realSons--;
        }
    }
    IS** new_sons= (IS**) calloc(realSons, sizeof(IS*));
    int k=0;
    for(i=0;i<realSons;i++){
        while(k<sonNumb){
            if(root->sons[k]!=NULL){
                new_sons[i]=root->sons[k];
                k++;
                break;
            }
            k++;
        }
    }
    free(root->sons);
    root->sons=new_sons;
    return realSons;
}



IS* two_seq(IS* f1, float min_sup){



}






char * spade(FILE* fp, float min_sup){
    printf("inside spade\n");
    IS* F1= one_seq(fp,min_sup);
    IS* F2= two_seq(F1,minsup);
/*
    Y* epsilon= equivalence_classes();//TODO da capire

    Z* sequences= enumerate_freq(epsilon);
*/

    return  NULL;
}




int main() {
    FILE *fp;

    fp  = fopen ("C:\\Users\\Kev\\Documenti\\zaki.txt", "r");

    if (fp == NULL)
        exit(EXIT_FAILURE);
    printf("file opened.\n");
    float min_sup=0.50;

    spade(fp, min_sup);

    fclose(fp);
    return 0;
}
