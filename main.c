#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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


unsigned int updateRelSup(IS *pSequence,unsigned int sonNumb,float min_sup,unsigned int tot);

IS* create2Seq(IS* precedent, IS* conseq,int equal);

//////////////////////////////////////////////////////////////////////////////////////////////////////

IS* ROOT=NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned int * encode(int val1, int val2, int* prefix, int prefix_length, int type, int equality, short prefix1, short prefix2){
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
                encoding[pos+1]=val1;
                encoding[pos+3]=3;
                encoding[pos+4]=1;
                encoding[pos+5]=2;
                encoding[pos+6]=val2;
                encoding[pos+7]=3;
                encoding[pos+8]=0;

            }
            else if(prefix1==1 && prefix2==0){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+5), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+1]=val2;
                encoding[pos+3]=3;
                encoding[pos+4]=0;
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
                encoding[pos+1]=val1;
                encoding[pos+3]=1;
                encoding[pos+4]=val2;
                encoding[pos+5]=3;
                encoding[pos+6]=0;

            }
            else if(prefix1==1 && prefix2==1){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+6), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=val1;
                encoding[pos+1]=1;
                encoding[pos+3]=val2;
                encoding[pos+4]=3;
                encoding[pos+5]=0;
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
                encoding[pos+1]=val1;
                encoding[pos+3]=3;
                encoding[pos+4]=0;

            }
            else if(prefix1==0 && prefix2==0){
                unsigned int *encoding = (unsigned int *) calloc((prefix_length+9), sizeof(unsigned int));
                unsigned int pos=0;
                for(pos;pos<prefix_length;pos++){
                    encoding[pos]=prefix[pos];
                }
                encoding[pos]=1;
                encoding[pos+1]=2;
                encoding[pos+1]=val2;
                encoding[pos+3]=3;
                encoding[pos+4]=1;
                encoding[pos+5]=2;
                encoding[pos+6]=val1;
                encoding[pos+7]=3;
                encoding[pos+8]=0;
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
        if(array[j]!=1 || array[j]!=2 || array[j]!=3){
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

/////////////////////////////////////////////////////////////////////////////////////
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
            fprintf(output_r,"sid: ");
            line *start = root->sons[i]->table->head;
            if (start == NULL) return 0;
            fprintf(output_r,"{");
            while (start->next != NULL) {
                fprintf(output_r,"%u, ", start->sid);
                start = start->next;
            }
            fprintf(output_r,"%u", start->sid, start->eid);
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


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

F1* one_seq(FILE* fp, float min_sup){
    unsigned int initial_sons=6;
    unsigned int oldsid;
    unsigned int sonNumb=0;
    unsigned int bypass[3]={2,3,0};


    IS* empty_set= (IS*) calloc(1, sizeof(IS));
    ROOT=empty_set;
    empty_set->id=id_counter;
    id_counter++;
    empty_set->prefix=0;
    empty_set->sequence=NULL;
    empty_set->support=0;
    empty_set->sons= (IS**) realloc(NULL, initial_sons*sizeof(IS*));


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
                //empty_set->sons[sonNumb]->id=id_counter;
                empty_set->sons[sonNumb]->encoded_sequence=bypass;
                //sprintf(empty_set->sons[sonNumb]->sequence,"{%s}",decompose);
                //empty_set->sons[sonNumb]->prefix=(char*) calloc(strlen(decompose), sizeof(char));
                //strcpy(empty_set->sons[sonNumb]->prefix,decompose);
                empty_set->sons[sonNumb]->prefix=1;


                //id_counter++;

                empty_set->sons[sonNumb]->sons=NULL;
                empty_set->sons[sonNumb]->nSons=0;


                empty_set->sons[sonNumb]->table=(TID*) calloc(1,sizeof(TID));
                //of sid is new allocate new spaces and append
                //empty_set->sons[sonNumb]->table->head=NULL;
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
        temp_id=(h+4);
        empty_set->sons[h]->id=temp_id;
        //TODO ENCODINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
        empty_set->sons[h]->encoded_sequence= encode(temp_id,0,NULL,0,1,0);//atom Y=0, prefix=NULL, prefix length =0, type=0
        //TODO ENCODINGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
    }

    id_counter=realsons+4;

    printer(empty_set,realsons);

    F1* result= (F1*) calloc(1, sizeof(F1));
    result->root=empty_set;
    result->atoms=empty_set->nSons;
    result->tot_sids=empty_set->support;
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


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

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


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
        atom1->sons=(IS**) realloc(atom1->sons, (2*(nAtoms-1)+1)*sizeof(IS*));
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
    newBorn2Seq->id=id_counter++;


    line* l1=precedent->table->head;
    line* l2;
    if(equal==0) {

//      newBorn2Seq->sequence=(char*) calloc(strlen(precedent->sequence)+strlen(conseq->sequence)+5, sizeof(char));//allocat memory for worst case in 2seq
//      sprintf(newBorn2Seq->sequence,"{%s},{%s}",precedent->sequence,conseq->sequence);

        newBorn2Seq->sequence=NULL;
        newBorn2Seq->encoded_sequence= encode(precedent->id,conseq->id,NULL,0,2,equal);

        newBorn2Seq->prefix=0;

        for (l1; l1 != NULL; l1 = l1->next) {
            for (l2=conseq->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next){
                if(l1->sid==l2->sid && l2->eid>l1->eid){
                    addLine(newBorn2Seq,l2->sid,l2->eid);
                }
            }

        }
    } else{

//       newBorn2Seq->sequence=(char*) calloc(strlen(precedent->sequence)+strlen(conseq->sequence)+3, sizeof(char));//allocat memory for worst case in 2seq
//       sprintf(newBorn2Seq->sequence,"{%s,%s}",precedent->sequence,conseq->sequence);

        newBorn2Seq->sequence=NULL;
        newBorn2Seq->encoded_sequence= encode(precedent->id,conseq->id,NULL,0,2,equal);
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
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

IS* createKSeq(IS* precedent, IS* conseq,int equal,int level){

    //char* seq2Name=(char*) calloc(strlen(precedent->sequence)+strlen(conseq->sequence)+5, sizeof(char));

    IS*newBorn2Seq=(IS*) calloc(1,sizeof(IS));

    newBorn2Seq->nSons=0;
    newBorn2Seq->support=0;
    newBorn2Seq->rsup=0;
    newBorn2Seq->sons=NULL;
    newBorn2Seq->table=NULL;
    newBorn2Seq->table=(TID*) calloc(1, sizeof(TID));
    newBorn2Seq->table->head=NULL;
    newBorn2Seq->id=id_counter++;


    //PREFIX EXTRACTION AND CHECKING
    //FIRST VERSION WITH STRING COMPARISON COMMENTED
    int i=0,j=0,counter1=level,counter2=level;
    while(counter1!=0 && precedent->encoded_sequence[i]!=0){
        if(precedent->encoded_sequence[i]==1) counter1--;
        i++;
    }
    while(counter2!=0 && conseq->encoded_sequence[j]!=0){
        if(conseq->encoded_sequence[j]==1) counter2--;
        j++;
    }
    int min= ((i<j)?i:j);
    unsigned int* pref1=(unsigned int*) calloc(min,sizeof(unsigned int));
    unsigned int* pref2=(unsigned int*) calloc(min,sizeof(unsigned int));

    memcpy(pref1,precedent->encoded_sequence,min);
    memcpy(pref2,conseq->encoded_sequence,min);

    if(memcmp(pref1,pref2,min)!=0) {
        free(newBorn2Seq->table);
        free(newBorn2Seq);
        return NULL;
    }
    unsigned int a= numEXTRACTOR(precedent->encoded_sequence+i);
    unsigned int b= numEXTRACTOR(conseq->encoded_sequence+j);


//    char* aa=(char*) calloc(strlen(precedent->sequence), sizeof(char));
//    char* bb=(char*) calloc(strlen(conseq->sequence), sizeof(char));
//    char* a=(char*) calloc(strlen(precedent->sequence), sizeof(char));
//    char* b=(char*) calloc(strlen(conseq->sequence), sizeof(char));

//    strcpy(aa,precedent->sequence);
//    strcpy(bb,conseq->sequence);

//    char * sep=strtok(aa+i," ,{}");
//    strcpy(a,sep);
//    strcpy(b,strtok(bb+j," ,{}"));
//


//    int i=0,j=0,counter1=level,counter2=level;
//    while(counter1!=0 && precedent->sequence[i]!='\0'){
//        if(precedent->sequence[i]==',') counter1--;
//        i++;
//    }
//    while(counter2!=0 && conseq->sequence[j]!='\0'){
//        if(conseq->sequence[j]==',') counter2--;
//        j++;
//    }
//    int min= ((i<j)?i:j);
//    char* pref1=(char*) calloc(min,sizeof(char));
//    char* pref2=(char*) calloc(min,sizeof(char));
//
//    snprintf(pref1,min,"%s",precedent->sequence);
//    snprintf(pref2,min,"%s",conseq->sequence);
//    if(strcmp(pref1,pref2)!=0){
//        free(newBorn2Seq->table);
//        free(newBorn2Seq);
//        return NULL;
//    }
//
//    char* aa=(char*) calloc(strlen(precedent->sequence), sizeof(char));
//    char* bb=(char*) calloc(strlen(conseq->sequence), sizeof(char));
//    char* a=(char*) calloc(strlen(precedent->sequence), sizeof(char));
//    char* b=(char*) calloc(strlen(conseq->sequence), sizeof(char));
//    strcpy(aa,precedent->sequence);
//    strcpy(bb,conseq->sequence);
//    char * sep=strtok(aa+i," ,{}");
//    strcpy(a,sep);
//    strcpy(b,strtok(bb+j," ,{}"));
//
    //CREATION OF THE SEQUENCES AND RELATIVE TABLES

    line* l1=precedent->table->head;
    line* l2;
    if(equal==2){
        line* l1=conseq->table->head;
        line* l2;
        if(precedent->prefix==0 && conseq->prefix==1) {
            newBorn2Seq->sequence=(char*) calloc(strlen(a)+strlen(conseq->sequence)+3, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s}", conseq->sequence, a);
            newBorn2Seq->prefix = 0;//////////////////////TODO SUPER UNDERSTAND WHAT TO PUT AS PREFIX EVERYWHERE
            for (l1; l1 != NULL; l1 = l1->next) {
                for (l2 = precedent->table->head; l2 != NULL && l2->sid <= l1->sid; l2 = l2->next) {
                    if (l1->sid == l2->sid && l2->eid > l1->eid) {
                        addLine(newBorn2Seq, l2->sid, l2->eid);
                    }
                }

            }
        }
        else if (precedent->prefix==0 && conseq->prefix==0){
            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+6, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s},{%s}", pref1, b,a);
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
            return NULL;
        }
    }
    else if(equal==0) {
        if (precedent->prefix==0 && conseq->prefix==0){
            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+6, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s},{%s}", pref1, a,b);
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
            return NULL;
        }

    }
    else{
        if(precedent->prefix==0 && conseq->prefix==0) {
            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+4, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,{%s,%s}",pref1, a, b);
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
            newBorn2Seq->sequence=(char*) calloc(strlen(pref1)+strlen(a)+strlen(b)+3, sizeof(char));//allocat memory for worst case in 2seq
            sprintf(newBorn2Seq->sequence, "%s,%s,%s}",pref1, a, b);
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
            return NULL;
        }

    }

    //strcpy(newBorn2Seq->sequence,seq2Name);
    //free(seq2Name);
    free(pref1);
    free(pref2);
    //free(a);
    //free(b);
    //free(aa);
    //free(bb);

    return newBorn2Seq;
}

int enumerate_freq(IS** f1, float min_sup,unsigned int nAtoms,unsigned int tot_sids, int level){

    if(f1==NULL || nAtoms==0) return 0;
    unsigned int TOTAL_SONS=0;
    //unsigned int allocate;
    IS* new_assignment;
    int i=0,j;
    IS* atom1=NULL;
    IS* atom2=NULL;

    for(j;j<nAtoms;j++){
        atom1=f1[i];

        new_assignment=NULL;
        if(!atom1->prefix) {
            if ((new_assignment = createKSeq(atom1, atom1, 0, level)) != NULL) {
                if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL) {
                    atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                    atom1->sons[atom1->nSons] = new_assignment;
                    atom1->nSons++;
                }
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
                if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL) {
                    atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                    atom1->sons[atom1->nSons] = new_assignment;
                    atom1->nSons++;
                }
            }
            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,2,level))!=NULL) {
                if(present(new_assignment->sequence,atom2,atom2->nSons)==NULL) {
                    atom2->sons = (IS **) realloc(atom2->sons, (atom2->nSons + 1) * sizeof(IS *));
                    atom2->sons[atom2->nSons] = new_assignment;
                    atom2->nSons++;
                }
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
                if(present(new_assignment->sequence,atom1,atom1->nSons)==NULL){
                    atom1->sons = (IS **) realloc(atom1->sons, (atom1->nSons + 1) * sizeof(IS *));
                    atom1->sons[atom1->nSons] = new_assignment;
                    atom1->nSons++;
                }
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
    //if(DFS){
        for(i=0;i<nAtoms;i++){
            atom1=f1[i];
            enumerate_freq(atom1->sons, min_sup, atom1->nSons, tot_sids, level + 1);
        }
    //}


/*




    for(i;i<nAtoms;i++){
        //allocate=1;
        //controlla che il tipo di prefisso sia consono
        atom1=f1[i];

        atom1->sons=(IS**) realloc(atom1->sons, (atom1->nSons+1)*sizeof(IS*));
        new_assignment=NULL;

        if(!atom1->prefix){
            if( (new_assignment=createKSeq(atom1,atom1,0,level))!=NULL) {
                //atom1->sons = (IS **) calloc(1, sizeof(IS *));
                atom1->sons[atom1->nSons] = new_assignment;
                atom1->nSons++;
            }
        }

        j=i+1;
        //if(j==nAtoms) j=0;

        //atom1->sons=(IS**) realloc(atom1->sons, (allocate)*sizeof(IS*));
        while(j<nAtoms){//TODO TOGLIERE CIRCOLARITA' j!=i

            atom2=f1[j];

            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,0,level))!=NULL){

                atom1->sons=(IS**) realloc(atom1->sons, (atom1->nSons+1)*sizeof(IS*));

                atom1->sons[atom1->nSons ]=new_assignment;
                atom1->nSons++;
            }
            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,1,level))!=NULL) {
                atom1->sons=(IS**) realloc(atom1->sons, (atom1->nSons+1)*sizeof(IS*));
                atom1->sons[atom1->nSons] = new_assignment;
                atom1->nSons++;
            }
            new_assignment=NULL;
            if((new_assignment=createKSeq(atom1,atom2,2,level))!=NULL) {
                atom2->sons=(IS**) realloc(atom2->sons, (atom2->nSons+1)*sizeof(IS*));
                atom2->sons[atom2->nSons] = new_assignment;
                atom2->nSons++;
            }

            j++;
            //if(j==nAtoms) j=0;
        }
        atom1->nSons=updateRelSup(atom1,atom1->nSons,min_sup,tot_sids);
        TOTAL_SONS=TOTAL_SONS+atom1->nSons;

        //////////////////////////////////////////////////////////////////////TODO DA DECIDERE
        //COME PULIRE LE MEMORIEEEEEEEEEEE-------- pulisco qui
        cleanTable(atom1->table);
        free(atom1->table);
        atom1->table=NULL;
        ///////////////////////////////////////////////////////////////////////////////
        if(DFS){
            //printf("alive %d,\n %s,\n %f\n",level,atom1->sequence,atom1->rsup);
            enumerate_freq(atom1->sons, min_sup, atom1->nSons, tot_sids, level + 1);
        }
    }
*/
/*
    if(BFS) {
        for(i=0;i<nAtoms;i++){
            cleanTable(f1[i]->table);
            free(f1[i]->table);
            f1[i]->table=NULL;
        }

        IS **T = (IS **) calloc(TOTAL_SONS, sizeof(IS *));
        int h = 0, p = 0, r = 0;
        for (p = 0; p < nAtoms; p++) {
            for (r; f1[p]->nSons; r++) {
                T[h] = f1[p]->sons[r];
                h++;
            }
        }

        enumerate_freq(T, min_sup, TOTAL_SONS, tot_sids, level + 1);
    }*//*

    for(i=0;i<nAtoms;i++){
        printer(f1[i],f1[i]->nSons);
    }
*/
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

    //enumerate(F1->root, min_sup, F1->atoms, F1->tot_sids);

    return  NULL;
}




int main(int argc, char *argv[]) {
    FILE *fp;

    //first command is file's absolute path to open

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



    spade(fp, min_sup);
    printf("SPADE TERMINATED RESULTS: %d.",max_id-1);

    fclose(fp);
    fclose(output_r);

    return 0;
}
