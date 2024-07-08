#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int data;
	int height;
	struct node *left;
	struct node *right;
}node;

int get_balance(node *);
int height(node *);
int max(int ,int );
node *min_node(node *);
node *create_node(int);
node *insert(node *,int);
node *delete_node(node *,int);
node *left_rotate(node *);
node *right_rotate(node *);
node *delete_tree(node *);
void inorder(node *);
int main(int argc,char *argv[]){
	node *root=NULL;
	root=insert(root,2);
	root=insert(root,1);
	root=insert(root,37);
	root=insert(root,47);
	root=insert(root,51);
	root=insert(root,332);
	root=insert(root,890);
	inorder(root);
	printf("\n");
	delete_node(root,47);
	inorder(root);
	printf("\n");
	delete_tree(root);
	return 0;
}
void inorder(node *t){
	if(t!=NULL){
		inorder(t->left);
		printf("%d ",t->data);
		inorder(t->right);
	}
}
node *delete_node(node *t,int d){
	if(root==NULL){
		return NULL;
	}
	else if(d<root->data){
		root->left=deleteNode(root->left,d);
	}
	else if(d>root->data){
		root->right=deleteNode(root->right,d);
	}
	else{
		if(root->left==NULL){
			node *temp=root->right;
			free(root);
			return temp;
		}
		else if(root->right==NULL){
			node *temp=root->left;
			free(root);
			return temp;
		}
		else{
			node *c=minValue(root->right);
			root->data=c->data;
			root->right=deleteNode(root->right,d);
		}
	}
	root->height=max(getHeight(root->left),getHeight(root->right))+1;
	int balance=getBalance(root);
	if(balance>1){
		if(getBalance(root->left)>=0){
			return rightRotate(root);
		}
		if(getBalance(root->left)<0){
			root->left=leftRotate(root->left);
			return rightRotate(root);
		}
	}
	if(balance<-1){
		if(getBalance(root->right)<=0){
			return leftRotate(root);
		}
		if(getBalance(root->right)>0){
			root->right=rightRotate(root->right);
			return leftRotate(root);
		}
	}
	return root;
}	
node *min_node(node *t){
	node *c=t;
	while(c!=NULL && c->left!=NULL){
		c=c->left;
	}
	return c;
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
        //update height
	t->height=max(height(t->left),height(t->right))+1;
	int balance=get_balance(t);

	if(balance>1){
		if(d<t->left->data){
			return right_rotate(t);
		}
		if(d>t->left->data){
			t->left=left_rotate(t->left);
			return right_rotate(t);
		}
	}
	if(balance<-1){
		if(d>t->right->data){
			return left_rotate(t);
		}
		if(d<t->right->data){
			t->right=right_rotate(t->right);
			return left_rotate(t);
		}
	}
	return t;
}
node *right_rotate(node *y){

	node *x=y->left;
	node *t=x->right;

	x->right=y;
	y->left=t;

	y->height=max(height(y->left),height(y->right))+1;
	x->height=max(height(x->left),height(x->right))+1;

	return x;
}
node *left_rotate(node *x){
	node *y=x->right;
	node *t=y->left;

	x->right=t;
	y->left=x;

	x->height=max(height(x->left),height(x->right))+1;
	y->height=max(height(y->left),height(y->right))+1;
	return y;
}
node *delete_tree(node *t){
	if(t!=NULL){
		delete_tree(t->left);
		delete_tree(t->right);
		free(t);
	}
	t=NULL;
}
node *create_node(int d){
	node *n=(node *)malloc(sizeof(node));
	if(n!=NULL){
		n->data=d;
		n->height=1;
		n->left=NULL;
		n->right=NULL;
		return n;
	}
	return NULL;
}
int max(int a,int b){
	return (a>b)?a:b;
}
int height(node *t){
	if(t==NULL){
		return 0;
	}
	return t->height;
}
int get_balance(node *t){
	if(t==NULL){
		return 0;
	}
	return height(t->left)-height(t->right);
}
