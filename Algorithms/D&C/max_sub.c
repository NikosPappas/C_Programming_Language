#include <stdio.h>
#include <limits.h>

int max_sub(int *,int,int);
int mid_sub(int *,int,int,int);

int main(int argc,char *argv[]){
	int a[]={13,-3,-25,20,-3,-16,-23,18,20,-7,12,-5,-22,15,-4,7};
        int r;
	int N=sizeof(a)/sizeof(a[0]);
	printf("Size of the array is :%d\n",N);
	r=max_sub(a,0,N-1);
	printf("sum:%d\n",r);
	return 0;
}
int mid_sub(int *a,int start,int med,int end){
      int sum=0;
      int left_sum=INT_MIN;
      int right_sum=INT_MIN;
      int i;
      for(i=med;i>=start;i--){
	      sum+=a[i];
	      if(sum>left_sum)
		      left_sum=sum;
      }
      sum=0;
      for(i=med+1;i<=end;i++){
	      sum+=a[i];
	      if(sum>right_sum)
		      right_sum=sum;
      }
      return max_three(left_sum+right_sum,left_sum,right_sum);

}
int max_sub(int *a,int start,int end){
    if(start>end){
	    return INT_MIN;
    }
    if(start==end){
	     return a[start];
     }
     else{
	     int med=(start+end)/2;
             int l=max_sub(a,start,med-1);
	     int r=max_sub(a,med+1,end);
	     int m=mid_sub(a,start,med,end);
             if(l>=r && l>=m)
		     return l;
	     else if(r>=l && r>=m)
		     return r;
	     else
		     return m;
     }
}
