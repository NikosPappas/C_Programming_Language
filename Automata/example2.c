#include <stdio.h>
#include <stdlib.h>
typedef enum states{
      before=0,
      s0,
      s1,
      s2,
      se
}states;

int main(int argc,char *argv[]){
 int c;
 states state=before;
 while((c=getchar())!=EOF){
        if(state==before){
           if(c==' '){
              printf("Moving from before-->S0\n");
              state=s0;
           }
         }
        if(state==s0){
            if(c=='r'){
             state=s1;
             printf("Moving from S0-->S1\n");
            }
         }
        if(state==s1){
           if(c>='0' && c<='9'){
               state=s2;
               printf("Moving from S1-->S2\n");
               c=getchar();
               }
         }
        if(state==s2){
            if(c>='0' && c<='9'){
               printf("Moving from S2-->S2\n");
               state=s2;
             }
        }
     }//end while loop
 return 0;
}
