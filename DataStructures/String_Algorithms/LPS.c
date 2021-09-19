#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void compute_lps(char *,int,int *);
void kmp_search(char *,char *);
int main(int argc,char *argv[]){
 char *pattern="abababbaa";
 int M=strlen(pattern);
 int *p=(int *)malloc(sizeof(int)*M);
 if(!p){
    perror("Failed to create array");
    return -1;
  }
 compute_lps(pattern,M,p);
 free(p);
 return 0;
}
void compute_lps(char *pat,int M,int *p){
   int i=1;
   int len=0;
   p[0]=0;
   while(i<M){
     if(pat[i]==pat[len]){
        len++;
        p[i]=len;
        i++;
     }//end if
     else{
        if(len!=0){
          len=p[len-1];
        }
        else{
        p[i]=0;
         i++;
        }
     }
   }//end while loop
   for(i=0;i<M;i++){
       printf("%d ",p[i]);
   }
   printf("\b\n");
}
