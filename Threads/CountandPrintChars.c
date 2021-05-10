#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* print_file(void *);
int main(){
  pthread_t t_id;
  pthread_create(&t_id,NULL,print_file,(void *)"/proc/cpuinfo");
  pthread_join(t_id,NULL);
  return 0;

}//end of main function

void *print_file(void *file_name){
  const char *name=(const char *)file_name;
  char c;
  FILE *f=NULL;
  long int counter=0;
  if(!(f=fopen(name,"r"))){
      fprintf(stderr,"Failed to open %s.Line %d",name,__LINE__);
      exit(0);
    }
  while((c=fgetc(f))!=EOF){
        counter++;
        fprintf(stdout,"%c",c);
   }
  printf("\nTotal characters %ld\n",counter);
  fclose(f);
  return NULL;
}

