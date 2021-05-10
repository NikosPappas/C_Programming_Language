#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* print_array(void*);
int main(){
  
  int a[]={10,5,6,3,4,8,7,9,1,2};
  pthread_t t_id;
  pthread_create(&t_id,NULL,print_array,(void *)a);
  pthread_join(t_id,NULL); 
  return 0;
}

void* print_array(void *received_arr){
    int *a=(int *)received_arr;
    int i;
    int j;
    int temp;
    printf("Sorting array using buble sort");
    for(i=0;i<10;i++){
       printf("%d ",a[i]); 
     }
    puts("\n");
    for(i=0;i<10;i++){
       for(j=9;j>i;j--){
          if(a[j]<a[j-1]){
             temp=a[j-1];
             a[j-1]=a[j];
             a[j]=temp;
          }
       }
    }//end of bubble sort;
    i=0;
    while(i<10){
     printf("%d ",a[i]);
      i++;
    }
    puts("\n");
}//end of function print_array
