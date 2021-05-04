#include<stdio.h>
#include<stdlib.h>
#include "node.h"

/* stack structure */
typedef struct Stack {
   int Capacity;   /* the max elements stack may hold */
   int TopOfStack; /* index to the top element */
   NODE *Array;    /* array holding elements */
} STACK;

void initStack(STACK *S,int capacity);
void push(STACK *S, int x);
int stackIsFull(STACK *S);

NODE pop(STACK *S);
int isEmptyStack(STACK *S);


//Stack Functions Implementation

   
   
NODE pop(STACK *S){
   NODE tempNode;
   
   tempNode=S->Array[S->TopOfStack];
   S->TopOfStack--;
   return tempNode;
}
int isEmptyStack(STACK *S){
   
   if (S->TopOfStack == -1)
     return 1;
   else return 0;
}  
void initStack(STACK *S, int capacity){
   int i;
   
   S->Capacity = capacity;
   S->TopOfStack = -1;
   S->Array = (NODE*) malloc(S->Capacity * sizeof(NODE));
   if (S->Array == NULL) {printf("error stack malloc!"); exit(1);}
}
void push(STACK *S, int x){
   
   //top++
   S->TopOfStack++;
   S->Array[S->TopOfStack].val=x;//value = x
}//end push

int stackIsFull(STACK *S){
   if (S->TopOfStack == S->Capacity - 1) 
      return 1;
   else return 0;
}
