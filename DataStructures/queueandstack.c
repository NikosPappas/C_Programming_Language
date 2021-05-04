#include <stdio.h>
#include <stdlib.h>

struct stack {
       int x;
       struct stack *next ;
       };

struct queue{
       int x;
       struct queue *next;
       };

void push(struct stack  **top,int data);
void pop(struct stack  **top);
int stackIsEmpty(struct stack  *top);

void enQueue(struct queue **front,struct queue **rear);
void init(struct queue **front,struct queue **rear);
int deQueue(struct queue **front,struct  queue **rear);
int  queueIsEmpty(struct queue *front);

void push(struct stack  **top,int data){
     struct  stack *newPtr=NULL;
     int val;
     newPtr=(struct stack *)malloc(sizeof(struct stack));
     if(!newPtr){
                 fprintf(stderr,"Malloc failed...\n");
                 return ;
                 }//telos if 
     else{
         
          newPtr->x=data;
          newPtr->next=NULL;
          if(stackIsEmpty(*top)){
                         (*top)=newPtr;

                         }//telos if 
          else{
               newPtr->next=(*top);
               (*top)=newPtr;
               
               }//telos else
          }//telos else
     }//telos push
     
int stackIsEmpty(struct stack *top){
    return top==NULL;
}

void pop(struct stack **top){
     struct stack  *temp=NULL;

     int popVal;
     
     temp=*top;
     popVal=temp->x;
     (*top)=(*top)->next;
     free(temp);
     printf("Pop Value: %d\n",popVal);
     
     }

void enQueue(struct queue **front,struct queue **rear){
     struct queue *newPtr=NULL;
     
     newPtr=(struct queue *)malloc( sizeof(struct queue) );
     if(!newPtr){
                 fprintf(stderr,"Malloc failed...\n");
                 return ;
                 
                 }
     else{
          printf("Dose timi: \n");
          scanf("%d",&(newPtr->x) );
          
          if( queueIsEmpty(*front) && queueIsEmpty(*rear) ){
                                *front=newPtr;
                                *rear=newPtr;
                                }
          else{
               (*rear)->next=newPtr;
               }
          
                (*rear)=newPtr;

          }
     }

void init(struct queue **front,struct queue **rear){
     *front=NULL;
     *rear=NULL;
     }
     
int deQueue(struct queue **front,struct  queue **rear){
     struct queue *temp=NULL;
     int qVal;
     temp=(*front);
     qVal=temp->x;
     *front=(*front)->next;
     
     if(*front==NULL)
              *rear=NULL;
     free(temp);
     return qVal;
     }
     
int queueIsEmpty(struct queue *front){
    return front==NULL;
    }
    
int main(void){
    struct queue *qPtr1,*qPtr2;
    struct stack *stackPtr=NULL;
    int k;
    
    init(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    enQueue(&qPtr1,&qPtr2);
    k=deQueue(&qPtr1,&qPtr1);
    push(&stackPtr,k);
    k=deQueue(&qPtr1,&qPtr1);
    push(&stackPtr,k);
    k=deQueue(&qPtr1,&qPtr1);
    push(&stackPtr,k);
    pop(&stackPtr);
    pop(&stackPtr);
    pop(&stackPtr);
    system("pause");
    
    
    return 0;
}

 
