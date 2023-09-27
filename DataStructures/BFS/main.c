#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct Q{
	int *a;
	int size;
	int front;
	int rear;
}Q;

typedef struct node{
	int vertex;
	struct node *next;
}node;
typedef struct adjList{
	node *head;
}adjList;
typedef struct graph{
	int num_vert;
	bool dir;
	adjList *adj;
}graph;

node *create_node(int);//done
graph *create_graph(int ,bool);//done
void add(graph *,int,int);
void print_graph(graph *);//done
void delete_graph(graph *);//done
void bfs(graph *,int);
void init_q(Q *,int);//done
void insert(Q *,int);
bool isEmpty(Q *);
int deq(Q *);//done
void delete_q(Q *);//done
int get_head(Q *);
int main(void){

	int kor=5;
	bool dir=true;
	graph *g=create_graph(kor,dir);
	add(g,0,4);
	add(g,0,1);
	add(g,1,2);
	add(g,1,4);
	add(g,1,3);
	add(g,3,4);
	add(g,4,0);
	print_graph(g);
	bfs(g,0);
	delete_graph(g);
	return 0;
}
int get_head(Q *t){
	return t->a[t->front];
}
void bfs(graph *g,int s){
          bool *visited=NULL;
	  Q t;
	  visited=malloc(sizeof(bool)*g->num_vert);
	  for(int i=0;i<g->num_vert;i++){
		  visited[i]=false;
	  }
	  init_q(&t,g->num_vert);
	  printf("=============BFS for vertex %d ========\n",s);
	  visited[s]=true;
	  insert(&t,s);
	  printf("Vertex: %d\n",s);
	  while(!isEmpty(&t)){
		  s=get_head(&t);
		  node *p=g->adj[s].head;
		  while(p){
			  if(!visited[p->vertex]){
				  visited[p->vertex]=true;
				  insert(&t,p->vertex);
				  printf("Vertex : %d\n");
			  }
			  p=p->next;
		  }
		  deq(&t);
	  }
}
void add(graph *g,int src,int dest){
	node *n=create_node(dest);
	n->next=g->adj[src].head;
	g->adj[src].head=n;
	
	if(!g->dir){
		n=create_node(src);
		n->next=g->adj[dest].head;
		g->adj[dest].head=n;
	}
}
void delete_graph(graph *g){
	if(g==NULL){
		printf("Graph is empty.Nothing to delete.\n");
		return ;
	}
	else{
		for(int i=0;i<g->num_vert;i++){
			printf("Deleting adjency list for vertex %d\n",i);
			node *head=g->adj[i].head;
			node *temp=NULL;
			while(head!=NULL){
				temp=head;
				head=head->next;
				free(temp);
			}
		}
	}
}
void print_graph(graph *g){
	if(g==NULL){
		printf("Graph is empty.Nothing to print\n");
		return ;
	}
	else{
		for(int i=0;i<g->num_vert;i++){
			printf("Adjency list for vertext %d: ",i);
			node *temp=g->adj[i].head;
			while(temp!=NULL){
				printf("%d ",temp->vertex);
				temp=temp->next;
			}
			printf("\n");
		}
	}
}
graph *create_graph(int num_vert,bool dir){
	graph *g=malloc(sizeof(graph));
	if(g){
		g->num_vert=num_vert;
		g->dir=dir;
		g->adj=malloc(sizeof(adjList)*num_vert);
		if(g->adj){
			for(int i=0;i<num_vert;i++){
				g->adj[i].head=NULL;
			}
		}
		else{
			printf("Error createing adjency list");
			return NULL;
		}
		return g;
	}
	else{
		printf("Error creating graph");
		return NULL;
	}
}
node *create_node(int v){
	node *n=malloc(sizeof(node));
	if(n){
		n->vertex=v;
		n->next=NULL;
		return n;
	}
	return NULL;
}
void delete_q(Q *t){
	free(t->a);
	t->size=0;
	t->front=-1;
	t->rear=-1;
}
bool isEmpty(Q *t){
	return t->front>t->rear || t->front==-1;
}
int deq(Q*t){
	if(t->front>t->rear || t->front==-1){
		t->rear=-1;
		printf("Queue is empty.\n");
		return -1;
	}
	else{
		int ret=t->a[t->front];
		t->front=t->front+1;
		return ret;
	}
}
void insert(Q *t,int vertex){
	if(t->front>t->size){
		printf("Queue overflow\n");
	}
	if(t->front==-1 && t->rear==-1){
		t->front++;
		t->rear++;
		t->a[t->rear]=vertex;
	}
	else{
		t->rear++;
		t->a[t->rear]=vertex;
	}
}
void init_q(Q *t,int size){
	t->size=size;
	t->front=-1;
	t->rear=-1;
	t->a=malloc(sizeof(int)*t->size);
	if(!t->a){
		printf("Error initializing q\n");
		return ;
	}
}

