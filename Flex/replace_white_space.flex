%option noyywrap
%{
#include<stdio.h>
#include <string.h>//for strcpy
#define N 1024
char file_name[N];
%}

%%
[ ]+ {fprintf(yyout," ");}
 
%%
int main( int argc, char **argv){
 FILE *f=NULL;
 f=fopen(argv[1],"r");
 if(f==NULL){
 fprintf(stderr,"Failed to open file %s",argv[1]);
 return -1;
 }
 yyrestart(f);
 yylex();
 fclose(f);
 return 0;
}

int yyrwap(){
 return 1;
}
