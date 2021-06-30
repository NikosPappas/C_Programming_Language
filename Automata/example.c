#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef enum states{
    state_a=0,
    state_b,
    state_c,
    state_d,
    error
}states;

int main(int argc,char *argv[]){
  int c;
  states state=state_a;
  while((c=getchar())!=EOF){
      if(state==state_a){
         if(c=='a'){
           printf("Moving from A-->B\n");
           state=state_b;
         }
         else{
           printf("Character is not in the alphabet\n");
         }
       }
      if(state==state_b){
          if(c=='b'){
            printf("Moving from B-->C\n");
            state=state_c;
           }
          if(c=='c'){
             printf("Moving from B-->D\n");
             state=state_d;
          }
       }
       if(state==state_c){
           if(c=='a'){
            printf("Moving from C-->B\n");
            state=state_b;
           }
       }
       if(state==state_d){
          if(c=='a'){
            printf("Moving from D-->B\n");
            state=state_b;
           }
        }
    }//end while loop
    return 0;
 }
        
