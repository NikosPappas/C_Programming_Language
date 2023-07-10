#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define size(x)  ((x).size)
#define first(x) (x.head->data)
#define last(x)  (x.tail->data)
typedef struct node{
	int data;
	struct node *next;
	struct node *prev;
}node;
typedef struct dq{
	node *head;
	node *tail;
	int size;
}dq;

node *create_node(int);//done
void insert(dq *,int);//done
void insert_first(dq *,int);
void insert_last(dq *,int);
void remove_first(dq *);
void remove_last(dq *);
void delete_dq(dq *);//done
void print_dq(dq *);//done
void print_reverse(dq *);//done
void init(dq *);//done
int main(int argc,char *argv[]){
	dq q;
	init(&q);
	insert(&q,10);
	insert(&q,20);
	insert(&q,30);
	printf("Size of list :%d\n",size(q));
	print_dq(&q);
	print_reverse(&q);
	printf("First element %d\n",first(q));
	printf("Last element %d\n",last(q));
	printf("Inserting  100 at head of the list \n");
	insert_first(&q,100);
	printf("First element %d\n",first(q));
	printf("Last element %d\n",last(q));
	printf("Inserting 200 as the last element\n");
	insert_last(&q,200);
	print_dq(&q);
	print_reverse(&q);
	printf("First element: %d\n",first(q));
	printf("Last element: %d\n",last(q));
	printf("Removing the last element...\n");
	remove_last(&q);
	printf("Size of the list %d\n",size(q));
	print_dq(&q);
	print_reverse(&q);
	printf("First element: %d\n",first(q));
	printf("Last element: %d\n",last(q));
	printf("Removing the first element...\n");
	remove_first(&q);
	printf("Size of the list %d\n",size(q));
	print_dq(&q);
	print_reverse(&q);
	delete_dq(&q);
	printf("Size of list :%d\n",size(q));
	return 0;
}
void insert_last(dq *q,int d){
	node *n=create_node(d);
	q->tail->next=n;
	n->prev=q->tail;
	q->tail=n;
	q->size++;
}
void remove_last(dq *q){
	node *prev=q->tail->prev;
	prev->next=NULL;
	free(q->tail);
	q->tail=prev;
	q->size--;
}
void insert_first(dq *q,int d){
	node *n=create_node(d);
	q->head->prev=n;
	n->next=q->head;
	q->head=n;
	q->size++;
}
void remove_first(dq *q){
	node *cur=q->head->next;
	cur->prev=NULL;
	free(q->head);
	q->head=cur;
	q->size--;
}
void delete_dq(dq *q){
	node *cur=NULL;
	while(q->head!=NULL){
		cur=q->head;
		q->head=q->head->next;
		free(cur);
		q->size--;
	}
	printf("\n");
}
void insert(dq *q,int data){
	node *n=create_node(data);
	if(q->head==NULL){
		q->head=n;
		q->size++;
	}
	else{
		node *cur=q->head;
		while(cur->next!=NULL){
			cur=cur->next;
		}
		cur->next=n;
		n->prev=cur;
		q->tail=n;
		q->size++;
	}
}
void print_reverse(dq *q){
	node *cur=q->tail;
	while(cur!=NULL){
		printf("%d ",cur->data);
		cur=cur->prev;
	}
	printf("\n");
}
void print_dq(dq *q){
     node *cur=q->head;
     while(cur!=NULL){
	     printf("%d ",cur->data);
	     cur=cur->next;
     }
     printf("\n");
}
void init(dq *q){
	q->head=NULL;
	q->tail=NULL;
	q->size=0;
}
node *create_node(int d){
	node *n=(node *)malloc(sizeof(node));
	assert(n!=NULL);
	n->data=d;
	n->next=NULL;
	n->prev=NULL;
	return n;
}
