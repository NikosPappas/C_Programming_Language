#include <stdio.h>

int main(){
 int i,n,fa=1,fb=1,fc;
 FILE *f=NULL;
 f=fopen("fibon.dat","w");
 do{
   printf("Type the last number of the sequence: ");
   scanf("%d",&n);
  }while(n<=2);
 fprintf(f,"\n%d\n%d",1,1);
 for(i=3;i<=n;i++){
   fc=fa+fb;
   fprintf(f,"\n%d",fc);
   fa=fb;
   fb=fc;
 }
 fclose(f);
 return 0;
}
