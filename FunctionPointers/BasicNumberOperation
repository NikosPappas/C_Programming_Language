#include <stdio.h>
#include <assert.h>
int sum(int, int);
int diff(int, int);
int mul(int, int);
int div(int, int);
int main(int argc, char*argv[]){
  int (*fp[4])(int,int)={NULL,NULL,NULL,NULL};
  int a,b;
  printf("Type the first integer: ");
  scanf("%d",&a);
  printf("Type the second integer: ");
  scanf("%d",&b);
  fp[0]=&sum;
  fp[1]=&diff;
  fp[2]=&mul;
  fp[3]=&div;
  printf("The sum of two integers is %d\n",fp[0](a,b));
  printf("The difference of the two integers is %d\n",fp[1](a,b));
  printf("The product of the two integers is %d\n",fp[2](a,b));
  printf("The division of the two integers is %d\n",fp[3](a,b));
  
 return 0;
}

int sum(int a, int b){
 return a+b;
}
int diff(int a, int b){
 return a-b;
}
int mul(int a, int b){
 return a*b;
}
int div(int a, int b){
 assert(b!=0);
 return a/b;
}

