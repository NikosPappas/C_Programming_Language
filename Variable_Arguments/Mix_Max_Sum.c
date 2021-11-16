int  sum (int count,...) {
int sum=0;
int i;
va_list valist;
va_start(valist,count);
for(i=0;i<count;i++){
    sum+=va_arg(valist,int);
}
va_end(valist);
 return sum;
}

int min(int count,...) {
 int min,a,i;
 va_list ap;
 va_start(ap,count);
 min=va_arg(ap,int);
 for(i=1;i<count;i++){
     if((a=va_arg(ap,int)<min)){
         min=a;
     }
 }
 va_end(ap);
 return min;
}

int max(int count,...) {
int i,max,a;
va_list ap;
va_start(ap,count);
max=va_arg(ap,int);
for(i=1;i<count;i++){
    if((a=va_arg(ap,int))>max){
        max=a;
    }
}
va_end(ap);
return max;
}
