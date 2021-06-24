#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
      int data;
      struct Node *next;
}Node;

void insert(Node **,int);
void delete(Node *);
void printList(Node *);
void sort(Node *);
int main(void){
   Node *head=NULL;
   Node *temp;
   insert(&head,10);
   insert(&head,1);
   insert(&head,3);
   insert(&head,9);
   insert(&head,11);
   insert(&head,2);
   printList(head);
   sort(head);
   printList(head); 
   //delete list 
   for(temp=head;temp!=NULL;temp=head->next){
    free(temp);
   }
 return 0;
}
void sort(Node *head){
  Node *i=NULL;
  Node *j=NULL;
  int temp;
  for(i=head;i!=NULL;i=i->next){
    for(j=i->next;j!=NULL;j=j->next){
        if(i->data>j->data){
          temp=i->data;
          i->data=j->data;
          j->data=temp;
         }
      }
    }
}
void insert(Node **head,int data){
   Node *newNode=NULL;
   //create a new node
   newNode=(Node *)malloc(sizeof(Node));
   newNode->data=data;
   newNode->next=NULL;
   if(*head==NULL){
      *head=newNode;
   }
   else{
    newNode->next=*head;
    *head=newNode;
   }
}


void printList(Node *head){
 Node *temp=head;
 while(temp!=NULL){
    printf("%d ",temp->data);
    temp=temp->next;
 }
 printf("\n");
}
