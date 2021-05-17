#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* print_file(void *);
void* count_chars(void *);
int main(){
  pthread_t t_id;
  pthread_t t2_id;
  pthread_create(&t_id,NULL,print_file,(void *)"/proc/cpuinfo");
  pthread_create(&t2_id,NULL,count_chars,(void*)"/proc/cpuinfo");
  pthread_join(t_id,NULL);
  pthread_join(t2_id,NULL);
  return 0;

}//end of main function

void *count_chars(void* file_name){
    FILE *f=NULL;
    long int counter=0;
    const char *name=(const char *)file_name;
    int c;
    f=fopen(name,"r");
    if(!f){
     fprintf(stderr,"Failed to open %s .",name);
     exit(0);
    }
    while((c=fgetc(f))!=EOF){
       counter++;
    }
    printf("Total characters in %s:%ld\n",name,counter);
    fclose(f);
}

void *print_file(void *file_name){
  const char *name=(const char *)file_name;
  char c;
  FILE *f=NULL;
  long int counter=0;
  pthread_mutex a_mutex;
  int rc=pthread_mutex_lock(a_mutex);
  if(rc){
    fprintf(stderr,"pthread_mutex_lock");
    pthread_exit(NULL);
  }
  if(!(f=fopen(name,"r"))){
      fprintf(stderr,"Failed to open %s.Line %d",name,__LINE__);
      exit(0);
    }
  while((c=fgetc(f))!=EOF){
        counter++;
        fprintf(stdout,"%c",c);
   }
  fclose(f);
  rc=pthread_mutex_unlock(&a_mutex);
  if(!rc){
    fprintf(stderr,"pthread_mutex_unlock");
    pthread_exit(NULL);
  }
  rc=pthread_mutex_destroy(&a_mutex);
  return NULL;
}
