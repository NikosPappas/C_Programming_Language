#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 1024
#define TABLE_SIZE 511

typedef struct node{
    char *word;
    int frequency;
    struct node *next;
}Node;
typedef Node* NodePtr;

typedef struct hash_t{
    int size;
    NodePtr *head;
}hash_t;
typedef hash_t* hashPtr;

NodePtr createNode(char *);
hashPtr initHash(void);
unsigned int hash(char *,int);
int insertHash(hashPtr,char *);
int findHash(hashPtr,char *);
int deleteHashTable(hashPtr );
void printHashTable(hashPtr );
void removePunct(char *,char *);

int main(void){
   
    char buffer[BUFFER_SIZE];
    char *token="";
    char *dest="";    
    int len;
    hashPtr t=initHash();
    FILE *f=fopen("sample2.txt","r");
    assert(f!=NULL);
    while(!feof(f)){
        fgets(buffer,BUFFER_SIZE,f);
        token=strtok(buffer," ");
        while(token){
            len=strlen(token);
            dest=(char *)malloc(sizeof(char)*(len+1));
            assert(dest!=NULL);
            removePunct(token,dest);
            insertHash(t,dest);
            token=strtok(NULL," ");
            free(dest);
        }
    }
    fclose(f);
    printHashTable(t);
    deleteHashTable(t);
}
void removePunct(char *s,char *d){
   while(*s){
       if(ispunct(*s)){
           s++;
       }
       else{
           *d++=*s++;
       }
   }
   *d='\0';
}
void printHashTable(hashPtr t){
    for(int i=0;i<t->size;i++){
        NodePtr cur=t->head[i];
        printf("Printing  hash list for head %d : ",i);
        while(cur!=NULL){
            printf("%s --> %d ",cur->word,cur->frequency);
            cur=cur->next;
        }
        printf("\n");
    }
}
int deleteHashTable(hashPtr t){
    for(int i=0;i<t->size;i++){
        NodePtr temp=NULL;
        while(t->head[i]!=NULL){
            temp=t->head[i];
            t->head[i]=t->head[i]->next;
            free(temp->word);
            free(temp);
        }
    }
}
int findHash(hashPtr t,char *s){
    unsigned int index=hash(s,t->size);
    NodePtr cur=t->head[index];
    while(cur!=NULL && strcmp(cur->word,s)!=0){
        cur=cur->next;
    }
    if(cur!=NULL){
        return 1;
    }
    return 0;
}
int insertHash(hashPtr t,char *s){
    unsigned int index=hash(s,t->size);
    NodePtr cur=NULL;
    NodePtr n=NULL;
    n=createNode(s);
    cur=t->head[index];
    if(cur==NULL){
        t->head[index]=n;
        return 1;
    }
    else{
        NodePtr prev=NULL;
        while(cur!=NULL && strcmp(cur->word,s)!=0){
            prev=cur;
            cur=cur->next;
        }
        if(cur!=NULL){
            cur->frequency+=1;
        }
        if(cur==NULL){
            prev->next=n;
        }
        return 1;
    }
}
unsigned int hash(char *s,int size){
    int len=strlen(s);
    unsigned int  h=0;
    int i;
    for(i=0;i<len;i++){
        h=(32*h+s[i])%size;
    }
    return h;
}
hashPtr initHash(void){
    hashPtr t=(hashPtr)malloc(sizeof(hash_t));
    assert(t!=NULL);
    t->size=TABLE_SIZE;
    t->head=(NodePtr *)malloc(sizeof(NodePtr)*t->size);
    assert(t->head!=NULL);
    for(int i=0;i<t->size;i++){
        t->head[i]=NULL;
    }
    return t;
}
NodePtr createNode(char *s){
    int len=strlen(s);
    NodePtr n=(NodePtr)malloc(sizeof(Node));
    assert(n!=NULL);
    n->next=NULL;
    n->word=(char *)malloc(sizeof(char)*(len+1));
    assert(n->word!=NULL);
    strcpy(n->word,s);
    n->frequency=1;
    return n;
}

