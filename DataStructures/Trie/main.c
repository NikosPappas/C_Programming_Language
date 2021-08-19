#include <stdio.h>
#include <stdlib.h>
#define N 26
typedef enum Boolean{
      false=0,
      true
}bool;

typedef struct trie {
       bool is_leaf;
       struct trie * children[N];
       char data;
}trie, *trie_ptr;
trie_ptr create_node(char );
trie_ptr insert_trie(trie_ptr, char *);
void free_trie(trie_ptr);
void print_trie(trie_ptr);
void print_search(trie_ptr,char *);
bool search(trie_ptr,char *);
int main(int argc,char *argv[]){
  //root node with N=26 nill childs;
  trie_ptr root = create_node('\0');
  root = insert_trie(root,"color");
  root = insert_trie(root,"all");
  root = insert_trie(root,"four");
  root = insert_trie(root,"hello");
  print_search(root,"color");
  print_search(root,"all");
  print_search(root,"four");
  print_search(root,"hello");
  print_search(root,"temp");
  print_trie(root);
  free_trie(root);
  return 0;
}
bool search(trie_ptr root,char *word){
    trie_ptr temp=root;
    int i;
    for(i=0;word[i]!='\0';i++){
        int index = word[i]-'a';
     if(temp->children[index]==NULL)
        return 0;
     temp=temp->children[index];
    }//end for loop
   if(temp!=NULL && temp->is_leaf==1)
     return 1;
  return 0;
}
void print_search(trie_ptr root,char *word){
   printf("Searching for %s: ",word);
   if(search(root,word)==0){
      printf("Not found\n");
   }
   else
      printf("Found!\n");
}
void print_trie(trie_ptr root){
   if(!root)
      return;
   trie_ptr temp=root;
   int i;
   printf("%c",temp->data);
   for(i=0;i<N;i++)
    print_trie(temp->children[i]);
}
void free_trie(trie_ptr root){
  int i;
  for(i=0;i<N;i++){
       if(root->children[i]!=NULL)
          free_trie(root->children[i]);
     }
   
  free(root);
}
trie_ptr create_node(char c){
    trie_ptr new_node=(trie_ptr)malloc(sizeof(trie));
    int i;
    for(i=0;i<N;i++)
       new_node->children[i]=NULL;
     new_node->data=c;
     new_node->is_leaf=false;
     return new_node;
}
trie_ptr insert_trie(trie_ptr root,char *word){
  trie_ptr temp = root;
  int i;
  for(i=0;word[i]!='\0';i++){
        //find the corrent index
        int idx=word[i]-'a';
       //check if child is null,ie doesn't have a letter
      //create a node if it is empty
      if(temp->children[idx]==NULL)
           temp->children[idx]=create_node(word[i]);
     temp=temp->children[idx];
  }
  temp->is_leaf=true;
  return root;
  
}

