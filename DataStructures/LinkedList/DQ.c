#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct link{
	int data;
	struct link *next;
}SL;

SL *create_node(int);
void append(SL **,int);
void print(SL*);
void freeList(SL *);
void insert_front(SL **,int);
void insert_last(SL **,int);
void remove_first(SL **);
void remove_last(SL **);
bool isEmpty(SL **);
void reverse_list(SL **);
int main(void){
        SL *t=NULL;
	append(&t,10);
	append(&t,30);
	append(&t,50);
	print(t);
	insert_front(&t,100);
	insert_front(&t,200);
	print(t);
	printf("Inserting last...");
	insert_last(&t,3);
	insert_last(&t,2);
	print(t);
	printf("Removing first element...\n");
	remove_first(&t);
	printf("Removing last element...\n");
	remove_last(&t);
	print(t);
	freeList(t);
	return 0;
}
void reverse_list(SL **l){
	SL *prev=NULL;
	SL *cur=*l;
	SL *next=NULL;
	while(cur!=NULL){
		next=cur->next;
		cur->next=prev;
		prev=cur;
		cur=next;
	}
	*l=prev;
}
bool isEmpty(SL **top){
	return *top==NULL;
}
void remove_first(SL **top){
	if(isEmpty(top)){
		printf("Linked list is empty.\n");
		return ;
	}
	SL *temp=*top;
	int t=(*top)->data;
	*top=(*top)->next;
	printf("%d  will be deleted.\n",t);
	free(temp);
}
void remove_last(SL **head){
	SL *t=*head;
	SL *prev=NULL;
	if(t==NULL){
		printf("Linked list is empty.\n");
	        return ;
	}
	
	while(t->next!=NULL){
		prev=t;
		t=t->next;
	}
	printf("%d will be deleted.\n",t->data);
	prev->next=NULL;
	free(t);
}
void insert_front(SL **head,int data){
         SL *n=create_node(data);
	 n->next=*head;
	 *head=n;
}
void insert_last(SL **head,int data){
	SL *t=*head;
	while(t->next!=NULL){
		t=t->next;
	}
	t->next=create_node(data);
}
void print(SL *head){
	SL *t=head;
	while(t!=NULL){
		printf("%d ",t->data);
		t=t->next;
	}
        printf("\n");
}
void freeList(SL *head){
	SL *temp=NULL;
	while(head!=NULL){
		temp=head;
		head=head->next;
		free(temp);
	}
	printf("Linked list is destroyed.\n");
}
void append(SL **head,int data){
	if(*head==NULL){
		*head=create_node(data);
	}
	else{
		SL *cur=*head;
		while(cur->next!=NULL){
			cur=cur->next;
		}
		cur->next=create_node(data);
	}
}
SL *create_node(int data){
	SL *n=(SL *)malloc(sizeof(int));
	if(!n){
		return NULL;
	}
	n->data=data;
	n->next=NULL;
	return n;
}
