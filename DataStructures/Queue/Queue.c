#include <stdio.h>
#include <stdlib.h>


struct queue{
       int x;
       struct queue *next;
       };
void enQueue(struct queue **front,struct queue **rear);
void init(struct queue **front,struct queue **rear);
void deQueue(struct queue **front,struct  queue **rear);
int  isEmpty(struct queue *front);

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
          
          if( isEmpty(*rear) ){
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
     
void deQueue(struct queue **front,struct  queue **rear){
     struct queue *temp=NULL;
     int qVal;
     temp=(*front);
     qVal=temp->x;
     *front=(*front)->next;
     
     if(*front==NULL)
              *rear=NULL;
     free(temp);
     printf("qVal: %d\n",qVal);
     }
int isEmpty(struct queue *front){
    return front==NULL;
    }

int main(void){
    struct queue *ptr1,*ptr2;
    init(&ptr1,&ptr2);
    
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    enQueue(&ptr1,&ptr2);
    deQueue(&ptr1,&ptr2);
    deQueue(&ptr1,&ptr2);
    deQueue(&ptr1,&ptr2);
    deQueue(&ptr1,&ptr2);
    getch();
    return 0;
}
