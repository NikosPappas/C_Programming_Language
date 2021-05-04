#include <stdio.h>
#include <stdlib.h>
#include <time.h>



struct tree{
       int x;          //timi komvou
       struct tree *left; //aristero paidi 
       struct tree *right; //deksio paidi
       };
       
struct list{
       int x;
       struct list *next;
       };
void insertList(struct list **head,int data);
void insertTree(struct tree **root,int data);
int  inOrder(struct tree *root);
void printList(struct list *head);




void insertList(struct list **head,int data){
     struct list *newPtr=NULL;
     struct list *cur=NULL;
     struct list *prev=NULL;
     newPtr=(struct list *)malloc(sizeof(struct list) );
     if(!newPtr){
                 fprintf(stderr,"Malloc failed....\n");
                 return ;
                 }
     else{
          newPtr->x=data;
          newPtr->next=NULL;
          
          if(*head==NULL){
                          *head=newPtr;
                          }
          else if(data<(*head)->x ) {
                                 newPtr->next=*head;
                                 *head=newPtr;
          
                                 }//telos else
          else{
                 cur=*head;
                 prev=*head;
                 while(cur!=NULL && data>cur->x){
                         prev=cur;
                         cur=cur->next;
                        }//telos while
                 /*eisagwgi metaksi komvwn*/
                    prev->next=newPtr;
                    newPtr->next=cur;
                    }    
                }//telos else
   }//telos insertList
void insertTree(struct tree **root,int data){     
     if(*root==NULL){//arxi if
                     *root=(struct tree *)malloc(sizeof(struct tree));
                     if( !(*root) ){//arxi if
                         fprintf(stderr,"Malloc failed..\n");
                         return ;
                         }//telos if
                     
                     else{//arxi else
                          (*root)->x=data;
                          (*root)->left=NULL;
                          (*root)->right=NULL;
                          }//telso else
                     
                     }//telos if
     else{
          if(data<(*root)->x){
                              insertTree( &( (*root)->left),data);
                              }
           else{
                 insertTree( & ( (*root)->right),data);
                }
           }
   } //telos insertTree
   
void printList(struct list *head){
     struct list *temp;
     temp=head;
     if(temp==NULL){
                    printf("List is empty.\n");
                    }
     else{
          while(temp!=NULL){
                            printf("%d \n ",temp->x);
                            temp=temp->next;
                            }//telos while
          }//telos else
}//telos printList

int inOrder(struct tree *root){
     if(root){
                inOrder(root->left);
                inOrder(root->right);
              }//telos if
     }//telos inOrder Traversal


int main(void){
    struct tree *treePtr=NULL;
    struct list *listPtr=NULL;
    int i;
    int k;
    int inOrderVal;
    
    srand(time(NULL));
    for(i=0 ; i<10; i++){
            k=rand()%1001;
    insertTree(&treePtr,k);
    }//telos for
    
    inOrderVal=inOrder(treePtr);
    
    insertList(&listPtr,inOrderVal);
    
    printList(listPtr);
    system("pause");
    return 0;   
}
