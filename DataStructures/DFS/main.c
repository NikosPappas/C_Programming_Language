#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct stack{
	int *a;
	int n;
	int top;
}Stack;

typedef struct node{
	int data;
	struct node *next;
}node;
typedef struct adjList{
	node *head;
}adjList;

typedef struct graph{
	int num_vert;
	adjList *adj;
	bool directed;
}graph;
int get_first(Stack *);
void init_stack(Stack *,int);//done
void push(Stack *,int);//done
int pop(Stack *);//done
bool isEmpty(Stack *);//done
void delete_stack(Stack *);
node *create_node(int);//done
graph *create_graph(int ,bool);//done
void add(graph *,int ,int );//done
void print_graph(graph *);//done
void delete_graph(graph *);
void dfs(graph *,int);;
int main(void){
        int ar=5;
	graph *g=create_graph(ar,true);
	add(g,0,4);
	add(g,0,1);
	add(g,1,2);
	add(g,1,4);
	add(g,1,3);
	add(g,3,4);
	add(g,4,0);
	dfs(g,0);
	print_graph(g);
	delete_graph(g);
	return 0;
}
int get_first(Stack *t){
	return t->a[t->top];
}
void delete_stack(Stack *t){
	t->n=0;
	t->top=-1;
	free(t->a);
	printf("Stack is deleted.\n");
}
void dfs(graph *g,int s){
	Stack t;
	bool *visited=NULL;
	
	visited=malloc(sizeof(bool)*g->num_vert);
	if(!visited){
		printf("Error creating array\n");
		return ;
	}
	for(int i=0;i<g->num_vert;i++){
		visited[i]=false;
	}
	init_stack(&t,g->num_vert);
	printf("==========DFS for vertex: %d======\n",s);
	visited[s]=true;
	push(&t,s);
	printf("Visited vertex %d \n",s);
	while(!isEmpty(&t)){
                 s=get_first(&t);
		 node *p=g->adj[s].head;
		while(p){
			if(visited[p->data]){
				p=p->next;
			}
			else{
				visited[p->data]=true;
				push(&t,p->data);
				printf("Visited vertex %d \n",p->data);
				p=g->adj[p->data].head;
			}
		}
		pop(&t);
	}
	delete_stack(&t);
	free(visited);
}
void print_graph(graph *g){
	if(g==NULL){
		printf("Nothing to print");
	}
	for(int i=0;i<g->num_vert;i++){
		printf("Adjancy list for %d vertex: ",i);
		node *head=g->adj[i].head;
		while(head!=NULL){
			printf("%d ",head->data);
			head=head->next;
		}
		printf("\n");
	}
}
void delete_graph(graph *g){
	if(g==NULL){
		printf("Graph is empty.Nothing to delete.\n");
		return ;
	}
	for(int i=0;i<g->num_vert;i++){
		printf("Delete adjancy list for vertext %d\n",i);
		node *head=g->adj[i].head;
		while(head!=NULL){
			node *temp=head;
			printf("deleting %d\n",temp->data);
			head=head->next;
			free(temp);
		}
		printf("==========\n");
	}

}
void add(graph *g ,int src,int dest){
	node *n=create_node(dest);
	n->next=g->adj[src].head;
	g->adj[src].head=n;
}
graph *create_graph(int num,bool dir){
	graph *g=malloc(sizeof(graph));
	if(!g){
		return NULL;
	}
	g->num_vert=num;
	g->directed=dir;
	g->adj=malloc(sizeof(adjList)*num);
	if(!g->adj){
		return NULL;
	}
	for(int i=0;i<g->num_vert;i++){
		g->adj[i].head=NULL;
	}
	return g;
}
node *create_node(int d){
	node *n=malloc(sizeof(node));
	if(n!=NULL){
		n->data=d;
		n->next=NULL;
		return n;
	}
	return NULL;
}
int pop(Stack *t){
	if(!isEmpty(t)){
		int p=t->a[t->top];
		--(t->top);
		return p;
	}
	else{
		printf("Stack is empty.\n");
		return -1;
	}
}
void push(Stack *t,int d){
	if(t->top<t->n){
		++(t->top);
		t->a[t->top]=d;
	}
	else{
		printf("Stack is full.");
	}
}
bool isEmpty(Stack *t){
	return t->top==-1;
}
void init_stack(Stack *t,int n){
	t->n=n;
	t->top=-1;
	t->a=malloc(sizeof(int)*n);
	if(!t->a){
		fprintf(stderr,"Failed to init stack.");
	}
}
