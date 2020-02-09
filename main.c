#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>

//GLOBAL VARIABLE IN THE WALL OF CODE
FILE* output_r;
unsigned int id_counter=3;
//int BFS=0;
//int DFS=1;
int VERBOSE=0;
int tdlists=0;
unsigned int max_id=1;


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
    char* literal;

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
IS* ROOT=(IS*)NULL;


unsigned int updateRelSup(IS *pSequence,unsigned int sonNumb,float min_sup,unsigned int tot);

IS* create2Seq(IS* precedent, IS* conseq,int equal);
/////////////////////////////////////////////////////////////////////////////////////
char * encode(unsigned int id){
    //CANT TAKE ID 0 AND 1
    //Note that the conversion is exact for an unsigned int up to the 53rd power of 2, for an IEEE754 double precision floating point type.

    int lengthHEX= (ceil(log(id)/log(16) +3));
    char * encoding=(char*) calloc( lengthHEX, sizeof(char));
    snprintf(encoding, lengthHEX, "%x", id );

    return encoding;
}

void convert(char* trans){

    fprintf(output_r,"{");
    char *token = strtok(trans, ",");

    long index= strtol(token, NULL, 16);
    fprintf(output_r,"%s", ROOT->sons[(index-2)]->literal);

    token = strtok(NULL, ",");
    // loop through the string to extract all other tokens
    while (token != NULL) {

        fprintf(output_r,",");
        index= strtol(token, NULL, 16);
        fprintf(output_r,"%s", ROOT->sons[(index-2)]->literal);
        token = strtok(NULL, ",");
    }
    fprintf(output_r,"}");

}
void decoding(char * input){
    char * seq= (char *) calloc(strlen(input)+1, sizeof(char));  //Modified
    strcpy(seq,input);
    int p=0, counter=0;
    unsigned int i=0;
    char* section=NULL;

    char** trans=NULL;
    while(seq[p]!='\0'){
        if(seq[p]=='}') counter++;
        p++;
    }
    if(counter==0){
        convert(seq);
    }
    else {
        trans = (char **) calloc(counter + (counter - 1), sizeof(char *));

//----------------------------------------------------
        char *token = strtok(seq, "{}");
        section = (char *) calloc(strlen(token)+1, sizeof(char));
        strcpy(section, token);
        trans[i] = section;
        i++;

        token = strtok(NULL, "{}");
        while (token != NULL) {
            section = (char *) calloc(strlen(token)+1, sizeof(char));
            strcpy(section, token);
            trans[i] = section;
            i++;
            token = strtok(NULL, "{}");
        }

        fprintf(output_r, "<");
        i = 0;
        convert(trans[i]);
        i++;
        for (i; i < (2 * counter - 1); i++) {
            if (strcmp(trans[i], ",") != 0) {
                fprintf(output_r, ",");
                convert(trans[i]);
            }
        }
        fprintf(output_r, ">");
        //cleaning
        for (i = 0; i < (2 * counter - 1); i++) {
            free(trans[i]);
        }
        free(trans);
        free(seq);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
/*
int printer(IS* root,unsigned int sN){
    int i=0;
    for(i;i<sN;i++){
        printf("=============================================\n");
        printf("SEQUENCE: %s\n=================\n",root->sons[i]->sequence);
        fprintf(output_r,"%d ",max_id);
        decoding(root->sons[i]->sequence);
        printf("support: %d\n rsup: %f\n-----------\n",root->sons[i]->support,root->sons[i]->rsup);
        max_id++;
        if(VERBOSE) {
            printf("V-idlist\n");
            line *start = root->sons[i]->table->head;
            if (start == NULL) return 0;

            while (start->next != NULL) {
                printf("sid: %u, eid: %u \n", start->sid, start->eid);
                start = start->next;
            }
            printf("sid: %u, eid: %u \n", start->sid, start->eid);
        }
    }
    return 0;
}*/

int printer(IS* root,unsigned int sN){
    int i=0;
    for(i;i<sN;i++){


        if(VERBOSE==0){
            fprintf(output_r,"%d ",max_id);
            decoding(root->sons[i]->sequence);

            //fprintf(output_r,"%d  <%s>  ",max_id,root->sons[i]->sequence);//TODO SISTEMARE LA PARTE ENCODED
            max_id++;
        }
        else if(VERBOSE==1){
            fprintf(output_r,"%d ",max_id);
            decoding(root->sons[i]->sequence);
            fprintf(output_r," rsup: %.3f  ",root->sons[i]->rsup);

            max_id++;
        }
        else if(VERBOSE==2){
            fprintf(output_r,"%d ",max_id);
            decoding(root->sons[i]->sequence);
            fprintf(output_r," sup: %d  ",root->sons[i]->support);
            fprintf(output_r," rsup: %.3f  ",root->sons[i]->rsup);

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
            fprintf(output_r,"sid: %u, eid: %u ", start->sid, start->eid);

        }
        else if(tdlists==1) {
            unsigned int xsid;
            fprintf(output_r," sid: ");
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
        fprintf(output_r,"\n");
    }
    return 0;
}
int addLine(IS* atom, int sid,int eid) {//costant
    line *lin = atom->table->tail;

    if (lin == NULL) {//first time head=tail
        atom->table->head = (line *) calloc(1, sizeof(line));
        atom->table->tail = atom->table->head;
        atom->table->tail->sid = sid;
        atom->table->tail->eid = eid;
        atom->support++;
        return 0;
    }
    if (lin->sid != sid) {
        atom->support++;
        lin->next = (line *) calloc(1, sizeof(line));
        lin->next->sid = sid;
        lin->next->eid = eid;
        atom->table->tail = lin->next;
        atom->table->tail->next = NULL;
    } else if (lin->eid !=
               eid) {//don't add the same lines all the time, since they are in order the last one is the biggest
        lin->next = (line *) calloc(1, sizeof(line));
        lin->next->sid = sid;
        lin->next->eid = eid;
        atom->table->tail = lin->next;
        atom->table->tail->next = NULL;
    }
    return 0;
}
/*//TODO if tooo slow we can save the tail and just pass the tail address for the add
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
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
    }
    else if(lin->eid!=eid){//don't add the same lines all the tieme, since they are in order the last one is the biggest
        lin->next= (line*) calloc(1, sizeof(line));
        lin->next->sid=sid;
        lin->next->eid=eid;
    }

    return 0;
}*/

//TODO DECIDE IF I NEED HASHING
IS* present(char* item, IS* root, unsigned int sonNumb){
    int i=0;
    //int r;
    IS* RESULT=NULL;
    for(i; i<sonNumb;i++){
        if(strcmp(root->sons[i]->literal,item)==0) {//r=
            RESULT=root->sons[i];
            return root->sons[i];
        }
    }
    return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

F1* one_seq(FILE* fp, float min_sup){
    int initial_sons=6;
    unsigned int oldsid;
    unsigned int sonNumb=0;


    IS* empty_set= (IS*) calloc(1, sizeof(IS));
    ROOT=empty_set;
    empty_set->id=id_counter;
    id_counter++;
    empty_set->prefix=0;
    empty_set->sequence=NULL;
    empty_set->support=0;
    empty_set->sons= (IS**) realloc(NULL, initial_sons*sizeof(IS*));

    //printf(("here"));
    char  bypass[5]="{bp}";
    char * line = NULL;
    size_t len = 100;
    //ssize_t read;

    while ((getline(&line, &len, fp)) != -1) { //todo cancellare le azioni verso read var
        char * field1= strtok(line,"{} \"");
        //char * copy_field1= (char*) calloc(strlen(field1), sizeof(char));
        //strcpy(copy_field1,field1);

//        printf("field1 is %s\n", field1);
//        printf("Retrieved line of length %zu:\n", read);
//        printf("%s\n", field1);
        int sid = atoi(strtok(NULL,"{},\""));

        if(empty_set->support==0){
            oldsid=sid;
            empty_set->support++;
        }
        else if(sid!=oldsid){
            oldsid=sid;
            empty_set->support++;
        }

        int eid = atoi(strtok(NULL,"{},\""));

        //printf("sid %d, eid %d \n", sid,eid);
        char * decompose= strtok(field1,",");

        while(decompose!=NULL){
            IS* atom=present(decompose,empty_set,sonNumb);

            if(atom==NULL){
                //case we adding a new element atom
                if(sonNumb>=initial_sons){
                    initial_sons= initial_sons*2;
                    empty_set->sons= (IS**) realloc(empty_set->sons, initial_sons*sizeof(IS*));
                }

                empty_set->sons[sonNumb] = (IS*) calloc(1, sizeof(IS));

                empty_set->sons[sonNumb]->literal=(char*) calloc(strlen(decompose)+1, sizeof(char));
                strcpy(empty_set->sons[sonNumb]->literal,decompose);

                empty_set->sons[sonNumb]->sequence=(char*) calloc(strlen(bypass)+1, sizeof(char));
                strcpy(empty_set->sons[sonNumb]->sequence,bypass);

                //sprintf(empty_set->sons[sonNumb]->sequence,"{%s}",decompose);fixo nella print?????????
                empty_set->sons[sonNumb]->prefix=1;

                //empty_set->sons[sonNumb]->id=id_counter;
                //id_counter++;

                empty_set->sons[sonNumb]->sons=NULL;
                empty_set->sons[sonNumb]->nSons=0;


                empty_set->sons[sonNumb]->table=(TID*) calloc(1,sizeof(TID));
                //of sid is new allocate new spaces and append
                empty_set->sons[sonNumb]->table->head=NULL;
                empty_set->sons[sonNumb]->table->tail=NULL;
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
    empty_set->nSons=realsons;

    //encoding based on the real position inside the root node
    unsigned int h=0;
    unsigned int temp_id=0;
    for(h=0;h<realsons;h++){
        temp_id=h;
        empty_set->sons[h]->id=temp_id;
        //TODO ENCODINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
        empty_set->sons[h]->sequence=encode(h + 2);
        //encode(temp_id,0,NULL,0,1,0,1,1);//atom Y=0, prefix=NULL, prefix length =0, type=0
        //TODO ENCODINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
    }

    id_counter=realsons;


    printer(empty_set,realsons);

    F1* result= (F1*) calloc(1, sizeof(F1));
    result->root=empty_set;
    result->atoms=empty_set->nSons;
    result->tot_sids=empty_set->support;
    return result;
}



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
        free(node->literal);
        free(node->table);
    }
}

unsigned int updateRelSup(IS* root, unsigned int sonNumb, float min_sup, unsigned int tot){
    int i=0;
    int realSons=sonNumb;
    int p,counter;

    for(i; i<sonNumb;i++){
        root->sons[i]->rsup=(float) root->sons[i]->support/tot;

        p=0,counter=tot; //checks sequence hasn't more transactions than sids
        while(root->sons[i]->sequence[p]!='\0'){
            if(root->sons[i]->sequence[p]=='}') counter--;
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



void two_seq(IS* f1, float min_sup,unsigned int nAtoms,unsigned int tot_sids){
    ///complexity A*N*(N-1)/2
    ///A instances of lines, N number of frequent atoms, worst case all of them
    unsigned int allocate=0;
    int i=0,j=0;
    IS* atom1=NULL;
    IS* atom2=NULL;

    for(j;j<nAtoms;j++){
        atom1=f1->sons[j];
        atom1->sons=(IS**) calloc(1,sizeof(IS*));
        atom1->sons[atom1->nSons ]=create2Seq(atom1,atom1,0);
        atom1->nSons++;
    }


    for(i;i<nAtoms;i++){
        j=i+1;
        if(j==nAtoms) j=0;
        atom1=f1->sons[i];
        atom1->sons=(IS**) realloc(atom1->sons, (2*(nAtoms-1)+1)*sizeof(IS*));//TODO????????????????????????
        while(j!=i){
            atom2=f1->sons[j];
            atom1->sons[atom1->nSons ]=create2Seq(atom1,atom2,0);
            atom1->nSons++;
            j++;
            if(j==nAtoms) j=0;
        }
    }
    for(i=0;i<nAtoms;i++){
        atom1=f1->sons[i];
        j=i+1;
        for(j;j<nAtoms;j++){
            atom2=f1->sons[j];
            atom1->sons[atom1->nSons ]=create2Seq(atom1,atom2,1);
            atom1->nSons++;
        }
    }

    for(i=0;i<nAtoms;i++){
        f1->sons[i]->nSons=updateRelSup(f1->sons[i],f1->sons[i]->nSons,min_sup,tot_sids);
    }
    for(i=0;i<nAtoms;i++){
        printer(f1->sons[i],f1->sons[i]->nSons);
    }
    for(i=0;i<nAtoms;i++){
        cleanTable(f1->sons[i]->table);
        free(f1->sons[i]->table);
        f1->sons[i]->table=NULL;
    }
}




IS* create2Seq(IS* precedent, IS* conseq,int equal){

    IS*newBorn2Seq=(IS*) calloc(1,sizeof(IS));

    newBorn2Seq->nSons=0;
    newBorn2Seq->support=0;
    newBorn2Seq->rsup=0;
    newBorn2Seq->sons=NULL;
    newBorn2Seq->table=NULL;
    newBorn2Seq->table=(TID*) calloc(1, sizeof(TID));
    newBorn2Seq->table->head=NULL;
    newBorn2Seq->table->tail=NULL;
    newBorn2Seq->literal=NULL;
    newBorn2Seq->id=id_counter++;


    line* l1=precedent->table->head;
    line* l2=NULL;
    if(equal==0) {

        newBorn2Seq->sequence=(char*) calloc(strlen(precedent->sequence)+strlen(conseq->sequence)+5+1, sizeof(char));//allocat memory for worst case in 2seq
        sprintf(newBorn2Seq->sequence,"{%s},{%s}",precedent->sequence,conseq->sequence);

        //newBorn2Seq->sequence=NULL;
        //newBorn2Seq->literal= encode(precedent->id,conseq->id,NULL,0,2,equal,1,1);

        newBorn2Seq->prefix=0;

        for (l1; l1 != NULL; l1 = l1->next) {
            for (l2=conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next){
                if(l1->sid==l2->sid && l2->eid>l1->eid){
                    addLine(newBorn2Seq,l2->sid,l2->eid);
                }
            }

        }
    } else{

        newBorn2Seq->sequence=(char*) calloc(strlen(precedent->sequence)+strlen(conseq->sequence)+3+1, sizeof(char));//allocat memory for worst case in 2seq
        sprintf(newBorn2Seq->sequence,"{%s,%s}",precedent->sequence,conseq->sequence);

        //newBorn2Seq->sequence=NULL;
        //newBorn2Seq->literal= encode(precedent->id,conseq->id,NULL,0,2,equal,1,1);

        newBorn2Seq->prefix=1;

        for (l1; l1 != NULL; l1 = l1->next) {
            for (l2=conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next){
                if(l1->sid==l2->sid && l2->eid==l1->eid){
                    addLine(newBorn2Seq,l2->sid,l2->eid);
                }
            }

        }
    }

    return newBorn2Seq;
}


IS* createKSeq(IS* precedent, IS* conseq,int equal,int level){

    IS*newBorn2Seq=(IS*) calloc(1,sizeof(IS));
    newBorn2Seq->sequence=NULL;
    newBorn2Seq->literal=NULL;
    newBorn2Seq->nSons=0;
    newBorn2Seq->support=0;
    newBorn2Seq->rsup=0;
    newBorn2Seq->sons=NULL;
    newBorn2Seq->table=NULL;
    newBorn2Seq->table=(TID*) calloc(1, sizeof(TID));
    newBorn2Seq->table->head=NULL;
    newBorn2Seq->table->tail=NULL;
    newBorn2Seq->id=id_counter++;


    int i=0,j=0,counter1=level,counter2=level;
    while(counter1!=0 && precedent->sequence[i]!='\0'){
        if(precedent->sequence[i]==',') counter1--;
        i++;
    }
    while(counter2!=0 && conseq->sequence[j]!='\0'){
        if(conseq->sequence[j]==',') counter2--;
        j++;
    }
    int min= ((i<j)?i:j);
    char* pref1=(char*) calloc(min,sizeof(char));
    char* pref2=(char*) calloc(min,sizeof(char));

    snprintf(pref1,min,"%s",precedent->sequence);
    snprintf(pref2,min,"%s",conseq->sequence);
    if(strcmp(pref1,pref2)!=0){
        free(newBorn2Seq->table);
        free(newBorn2Seq);
        return NULL;
    }

    char* aa=(char*) calloc(strlen(precedent->sequence)+1, sizeof(char));
    char* bb=(char*) calloc(strlen(conseq->sequence)+1, sizeof(char));
    char* a=(char*) calloc(strlen(precedent->sequence)+1, sizeof(char));
    char* b=(char*) calloc(strlen(conseq->sequence)+1, sizeof(char));
    strcpy(aa,precedent->sequence);
    strcpy(bb,conseq->sequence);
    char * sep=strtok(aa+i," ,{}");
    strcpy(a,sep);
    strcpy(b,strtok(bb+j," ,{}"));

    //CREATION OF THE SEQUENCES AND RELATIVE TABLES

    line* head_search=NULL;
    line* tail_search=NULL;

    line* l1=precedent->table->head;
    line* l2=NULL;
    if(equal==2){

        line* l1=conseq->table->head;

        if(precedent->prefix==0 && conseq->prefix==1) {

            newBorn2Seq->sequence=(char*) calloc(strlen(a)+strlen(conseq->sequence)+3+1, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s}", conseq->sequence, a);
            //newBorn2Seq->literal=encode(a,b,conseq->literal,lenarray(conseq->literal),3,equal,precedent->prefix,conseq->prefix);
            newBorn2Seq->prefix = 0;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE

            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = precedent->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid > l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
            /*head_search= l2;
            tail_search= l2;

            for (l1; l1 != NULL; l1 = l1->next) {

                if(tail_search==NULL || l1->sid==tail_search->sid ){
                    head_search=tail_search;
                }
                else{
                    tail_search=head_search;
                }
                for (tail_search; tail_search != NULL && tail_search->sid <= l1->sid; tail_search = tail_search->next) {
                    if (l1->sid == tail_search->sid && tail_search->eid > l1->eid) {
                        addLine(newBorn2Seq, tail_search->sid, tail_search->eid);
                    }
                }

            }*/
        }
        else if (precedent->prefix==0 && conseq->prefix==0){

            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+6+1, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s},{%s}", pref1, b,a);
            //newBorn2Seq->literal=encode(a,b,pref1,min,3,equal,precedent->prefix,conseq->prefix);
            newBorn2Seq->prefix = 0;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = precedent->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid > l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }else{
            free(newBorn2Seq->sequence);
            free(newBorn2Seq->table);
            free(newBorn2Seq);
            free(pref1);
            free(pref2);
            free(a);
            free(b);
            free(aa);
            free(bb);
            return NULL;
        }
    }
    else if(equal==0) {
        if (precedent->prefix==0 && conseq->prefix==0){

            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+6+1, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s},{%s}", pref1, a,b);
            //newBorn2Seq->literal=encode(a,b,pref1,min,3,equal,precedent->prefix,conseq->prefix);

            newBorn2Seq->prefix = 0;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid > l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }
        else if(precedent->prefix==1 && conseq->prefix==0) {

            newBorn2Seq->sequence=(char*) calloc(strlen(b)+strlen(precedent->sequence)+3, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s}", precedent->sequence, b);
            //newBorn2Seq->literal=encode(a,b,precedent->literal,lenarray(precedent->literal),3,equal,precedent->prefix,conseq->prefix);

            newBorn2Seq->prefix = 0;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid == l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }
        else{
            free(newBorn2Seq->sequence);
            free(newBorn2Seq->table);
            free(newBorn2Seq);
            free(pref1);
            free(pref2);
            free(a);
            free(b);
            free(aa);
            free(bb);
            return NULL;
        }

    }
    else{
        if(precedent->prefix==0 && conseq->prefix==0) {

            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+4+1, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s,%s}",pref1, a, b);
            //newBorn2Seq->literal=encode(a,b,pref1,min,3,equal,precedent->prefix,conseq->prefix);

            newBorn2Seq->prefix = 1;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid == l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }
        else if(precedent->prefix==1 && conseq->prefix==1) {

            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+3+1, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,%s,%s}",pref1, a, b);
            //newBorn2Seq->literal=encode(a,b,pref1,min,3,equal,precedent->prefix,conseq->prefix);

            newBorn2Seq->prefix = 1;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid == l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }
        else{
            free(newBorn2Seq->sequence);
            free(newBorn2Seq->table);
            free(newBorn2Seq);
            free(pref1);
            free(pref2);
            free(a);
            free(b);
            free(aa);
            free(bb);
            return NULL;
        }

    }


    free(pref1);
    free(pref2);
    free(a);
    free(b);
    free(aa);
    free(bb);

    return newBorn2Seq;
}

int enumerate_freq(IS** f1, float min_sup,unsigned int nAtoms,unsigned int tot_sids, int level){

    if(f1==NULL || nAtoms==0) return 0;
    //unsigned int TOTAL_SONS=0;
    //unsigned int allocate;
    IS* new_assignment = NULL;
    int i=0,j=0;
    IS* atom1=NULL;
    IS* atom2=NULL;

    for(j;j<nAtoms;j++){
        atom1=f1[j];

        //new_assignment=NULL;
        if(!atom1->prefix) {
            if ((new_assignment = createKSeq(atom1, atom1, 0, level)) != NULL) {
                //if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL) {
                atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                atom1->sons[atom1->nSons] = new_assignment;
                atom1->nSons++;
                //}
            }
        }
    }


    for(i;i<nAtoms;i++){
        j=i+1;
        //if(j==nAtoms) j=0;
        atom1=f1[i];

        while(j<nAtoms){//j!=i
            atom2=f1[j];

            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,0,level))!=NULL) {
                //if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL) {
                atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                atom1->sons[atom1->nSons] = new_assignment;
                atom1->nSons++;
                //}
            }
            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,2,level))!=NULL) {
                //if(present(new_assignment->sequence,atom2,atom2->nSons)==NULL) {
                atom2->sons = (IS **) realloc(atom2->sons, (atom2->nSons + 1) * sizeof(IS *));
                atom2->sons[atom2->nSons] = new_assignment;
                atom2->nSons++;
                //}
            }
            j++;
            //if(j==nAtoms) j=0;
        }
    }
    for(i=0;i<nAtoms;i++){
        atom1=f1[i];
        j=i+1;
        for(j;j<nAtoms;j++){
            atom2=f1[j];

            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,1,level))!=NULL) {
                //if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL){
                atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                atom1->sons[atom1->nSons] = new_assignment;
                atom1->nSons++;
                //}
            }
        }
    }

    for(i=0;i<nAtoms;i++){
        f1[i]->nSons=updateRelSup(f1[i],f1[i]->nSons,min_sup,tot_sids);
    }
    for(i=0;i<nAtoms;i++){
        printer(f1[i],f1[i]->nSons);
    }
    for(i=0;i<nAtoms;i++){
        cleanTable(f1[i]->table);
        free(f1[i]->table);
        f1[i]->table=NULL;
    }

    for(i=0;i<nAtoms;i++){
        atom1=f1[i];
        enumerate_freq(atom1->sons, min_sup, atom1->nSons, tot_sids, level + 1);
    }


    return 1;
}


void enumerate(IS* f1, float min_sup, unsigned int nAtoms, unsigned int tot_sids){
    unsigned int i=0;
    for(i;i<nAtoms;i++){//for equivalence class
        enumerate_freq(f1->sons[i]->sons,min_sup,f1->sons[i]->nSons,tot_sids,1);
    };

}

char * spade(FILE* fp, float min_sup){
    printf("inside spade\n");

    F1* F1= one_seq(fp,min_sup);

    two_seq(F1->root,min_sup,F1->atoms,F1->tot_sids);

    enumerate(F1->root, min_sup, F1->atoms, F1->tot_sids);

    return  NULL;
}




int main(int argc, char *argv[]) {
    FILE *fp;
    printf("WELCOME TO SPADE\n");
    //first command is file's absolute path to open
    //printf("%d argc",argc);
    //fp  = fopen ("C:\\Users\\Kev\\Documenti\\zaki.txt", "r");
    //output_r= fopen ("C:\\Users\\Kev\\Documenti\\OREEEELALE.txt", "w");
    // ARGV[1] INPUT
    //ARGV[2] OUTPUT

    fp  = fopen (argv[1], "r");
    output_r= fopen (argv[2], "w");

    if (fp == NULL || output_r==NULL)
        exit(EXIT_FAILURE);
    printf("files opened.\n");

    float min_sup=atof(argv[3]);
    VERBOSE=atoi(argv[4]);
    tdlists=atoi(argv[5]);


    clock_t start = clock();
    spade(fp, min_sup);
    clock_t stop = clock();

    printf("SPADE TERMINATED RESULTS: %d.",max_id-1);

    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;

    fclose(fp);
    fclose(output_r);

    printf("\nTime elapsed: %f\n", elapsed);


    return 0;
}