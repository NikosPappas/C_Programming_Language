#include <stdio.h>//standard I/O functions
typedef enum states{
       start=0,
       s0,
       s1,
       final_state
}states;
states start_state(int);
states s0_state(int);
states s1_state(int);
states final_states(int);

int main(int argc,char *argv[]){
 int c;
 states state=start;
 while((char)(c=getchar())!=EOF){
      if(state==start){
       state=start_state(c);
      }
      else if(state==s0){
        state=s0_state(c);
      }
      else if(state==s1){
        state=s1_state(c);
      }
      else if(state==final_state){ 
        fprintf(stdout,"string found\n");
        state=final_states(c);
     }  
 } 
 return 0;
}

states start_state(int c){
 states st;
 if((char)c=='b'){
      st=start;
      printf("start-->start\n");
 }
 else if((char)c=='a'){
    st=s0;
    printf("start-->s0\n");
 }
 return st;
}

states s0_state(int c){
 states st;
  if((char)c=='a'){
     st=s1;
     printf("s0-->s1\n");
  }
  else if((char)c=='b'){
     st=start;
     printf("s0-->start\n");
   }
 return st;
}

states s1_state(int c){
 states st;
 if((char)c=='a'){
     st=s1;
      printf("s1-->s1\n");
 }
 else if((char)c=='b'){
     st=final_state;
     printf("s1-->final_state\n");
 }
 else{
    st=start;
 }
 return st;
}

states final_states(int c){
  states st;
  if((char)c=='a'){
     st=final_state;
     printf("final_state-->final_state\n");
  }
  else if((char)c=='b'){
    st=final_state;
   printf("final_state-->final_state\n");
  }
  return st;
}
