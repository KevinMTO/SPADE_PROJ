#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<wchar.h>

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

    int support;
    float rsup;

    TID * table;
    struct sequence** sons;
}typedef IS;



/////////////////////////////////////////////////////////////////////////////////////
int printer(IS* root,unsigned int sN){
    int i=0;
    for(i;i<sN;i++){

        printf("SEQUENCE: %s\n",root->sons[i]->sequence);
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

//if tooo slow we can save the tail and just pass the tail address for the add
int addLine(TID* table, int sid,int eid){

    line* lin = table->tableline;

    if(lin==NULL){
        table->tableline= (line*) calloc(1, sizeof(line));
        table->tableline->sid=sid;
        table->tableline->eid=eid;
        return 0;
    }

    while(lin->next!=NULL){
        lin = lin->next;
    }

    lin->next= (line*) calloc(1, sizeof(line));
    lin->next->sid=sid;
    lin->next->eid=eid;
    return 0;
}

IS* present(char* item, IS* root, unsigned int sonNumb){
    int i=0;
    int r;
    IS* RESULT=NULL;
    for(i; i<sonNumb;i++){
        if(r=strcmp(root->sons[i]->sequence,item)==0) {
            RESULT=root->sons[i];
            return root->sons[i];
        }
    }
    return NULL;
}

IS* one_seq(FILE* fp, int min_sup){
    int initial_sons=11;
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

    printf(("here"));
    char * line = NULL;
    size_t len = 100;
    //ssize_t read;

    while ((getline(&line, &len, fp)) != -1) { //todo cancellare le azioni verso read var
        char * field1= strtok(line,"{ } \" ");
        char * copy_field1= (char*) calloc(strlen(field1), sizeof(char));
        strcpy(copy_field1,field1);

//        printf("field1 is %s\n", field1);
//        printf("Retrieved line of length %zu:\n", read);
//        printf("%s\n", field1);
        int sid = atoi(strtok(NULL,"{ }, \"  "));

        if(empty_set->support==0){oldsid=sid;}
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

                    addLine(empty_set->sons[sonNumb]->table,sid,eid);

                    sonNumb++;
            }
            else{
                    //based on asssumption that we're reading data already order
                    addLine(atom->table,sid,eid);
            }

            decompose= strtok(NULL,", ");

        }


    }
    printer(empty_set,sonNumb);
return NULL;
}



IS* two_seq(IS* f1, int min_sup){

}






char * spade(FILE* fp, int minsup){
    printf("inside spade");
    IS* F1= one_seq(fp,minsup);
    //IS* F2= two_seq(F1,minsup);
/*
    Y* epsilon= equivalence_classes();//TODO da capire

    Z* sequences= enumerate_freq(epsilon);
*/

    return  NULL;
}




int main() {
    FILE *fp;
    printf("ciao");
    fp  = fopen ("C:\\Users\\Kev\\Documenti\\zaki.txt", "r");
    printf("11111");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    printf("2222");
    int min_sup=0;

    spade(fp, min_sup);

    fclose(fp);
    return 0;
}
