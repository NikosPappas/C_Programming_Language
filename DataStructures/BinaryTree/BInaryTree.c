#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int data;
	struct node *left;
	struct node *right;
}
node;
node *create_node(int);//DONE
node *insert(node *,int);
node *delete_node(node *,int);//done
node *min_node(node *);//done
void delete_tree(node *);//done
void inorder(node *);//done
void postorder(node *);//done
void preorder(node *);//done

int main(int argc,char *argv[]){

	node *t=NULL;
	t=insert(t,10);
	t=insert(t,5);
	t=insert(t,1);
	t=insert(t,50);
	t=insert(t,4);
        inorder(t);
	printf("\n");
	delete_node(t,10);
	inorder(t);
	printf("\n");
	delete_tree(t);
	if(t!=NULL){
		t=NULL;
	}
	return 0;
}
void delete_tree(node *t){
	if(t!=NULL){
		delete_tree(t->left);
		delete_tree(t->right);
		free(t);
	}
}
node *delete_node(node *t,int k){
	if(t==NULL){
		return NULL;
	}
	else if(t->data==k){
		node *c=min_node(t->right);
		t->data=c->data;
		t->right=delete_node(t->right,k);
		t->right=NULL;
	}
	else if(k<t->data){
		t->left=delete_node(t->left,k);
	}
	else if(k>t->data){
		t->right=delete_node(t->right,k);
	}
	else{
		if(t->left==NULL){
			node *temp=t->right;
			free(t);
			return temp;
		}
		else if(t->right==NULL){
			node *temp=t->left;
			free(t);
			return temp;
		}
		else{
			node *c=min_node(t->right);
			t->data=c->data;
			t->right=delete_node(t->right,c->data);

		}
	}
	if(t==NULL){
		return t;
	}
	return t;
}
node *min_node(node *t){
	if(t!=NULL){
		node *c=t;
		while(c->left!=NULL){
			c=c->left;
		}
		return c;
	}
	return NULL;
}
node *insert(node *t,int d){
	if(t==NULL){
		return create_node(d);
	}
	else if(d<t->data){
		t->left=insert(t->left,d);
	}
	else if(d>t->data){
		t->right=insert(t->right,d);
	}
	else{
		return t;
	}
	return t;
}
void inorder(node *t){
	if(t!=NULL){
		inorder(t->left);
		printf("%d ",t->data);
		inorder(t->right);
	}
}
node *create_node(int k){
	node *n=(node *)malloc(sizeof(node));
	if(n!=NULL){
		n->data=k;
		n->left=NULL;
		n->right=NULL;
		return n;
	}
	return NULL;
}
