#include <stdio.h>
#include <stdlib.h>

typedef struct node 
{
	int data;
	struct node *left;
	struct node *right;
}Tree,*TreePtr;

static int lookup(struct node*,int);
TreePtr NewNode(int);
int Size(TreePtr);
int MinValue(TreePtr);
void FreeRoot(TreePtr);
int MaxDepth(TreePtr);
void PrintTree(TreePtr);
int main(int argc,char **argv)
{
	TreePtr root=NewNode(10);
	root->left=NewNode(9);
	root->right=NewNode(11);
	printf("Minimum value:%d.\n",MinValue(root));
	printf("Size of tree is %d.\n",Size(root));
	PrintTree(root);
	//FreeRoot(root->left);
	FreeRoot(root);
	getchar();
	return 0;
}
void PrintTree(TreePtr root)
{
	if(root!=NULL)
	{
		PrintTree(root->left);
		printf("%d ",root->data);
		PrintTree(root->right);
	}
}

int MaxDepth(TreePtr root)
{
	if(root==NULL)
		return 0;
	else
	{
		int lDepth=MaxDepth(root->left);
		int rDepth=MaxDepth(root->right);
		return (lDepth > rDepth) ? (lDepth+1):(rDepth+1);
	}
}
int MinValue(TreePtr root)
{
	TreePtr temp=root;

	while(temp->left!=NULL)
	{
		temp=root->left;
	}
	return temp->data;
}
void FreeRoot(TreePtr root)
{
	free(root->left);
	free(root->right);
	free(root);
}
int Size(TreePtr root)
{
	if(root==NULL)
	{
		return 0;
	}
	else
	{
		return (Size(root->left)+1+Size(root->right));
	}
}
TreePtr NewNode(int data)
{
	TreePtr newNode=(TreePtr)malloc(sizeof(Tree));
	newNode->data=data;
	newNode->left=NULL;
	newNode->right=NULL;
	return newNode;
}

static int lookup(struct node* node, int target) 
{
	if (node == NULL)
	{
		return(false);
	}
	else 
	{
		if (target == node->data) return(true);
		else 
		{
			if (target < node->data) 
				return(lookup(node->left, target));
            else 
				return(lookup(node->right, target));
        }
    }
} 
