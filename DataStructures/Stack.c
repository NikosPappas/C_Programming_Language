#include <stdio.h>
#include <stdlib.h>

struct stack {
       int x;
       struct stack *next ;
       };


void push(struct stack  **top);
void pop(struct stack  **top);
int isEmpty(struct stack  *top);

void push(struct stack  **top){
     struct  stack *newPtr=NULL;
     int val;
     newPtr=(struct stack *)malloc(sizeof(struct stack));
     if(!newPtr){
                 fprintf(stderr,"Malloc failed...\n");
                 return ;
                 }//telos if 
     else{
         
          printf("Dose tin timi: \n");
          scanf("%d", &(newPtr->x) );
          newPtr->next=NULL;
          if(isEmpty(*top)){
                         (*top)=newPtr;
                         printf("Eisagwgi Prwti timis OK!!!!\n");
                         }//telos if 
          else{
               newPtr->next=(*top);
               (*top)=newPtr;
               printf("Eisagwgi Allwn Timwn OK!!!\n");
               }//telos else
          }//telos else
     }//telos push
int isEmpty(struct stack *top){
    return top==NULL;
}

void pop(struct stack **top){
     struct stack  *temp=NULL;

     int popVal;
     
     temp=*top;
     popVal=temp->x;
     (*top)=(*top)->next;
     free(temp);
     printf("Pop Value: %d---->%d\n",popVal,&popVal);
     
     }
 
 
int main(void){
    struct stack  *ptr1=NULL;
    int k;
    
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    push(&ptr1);
    /**/
    while(!isEmpty(ptr1));
    pop(&ptr1);
    getch();   
    return 0;
}
     
          
