%option noyywrap
%{
int chars=0;
int words=0;
int lines=0;
%}
%%
[a-zA-Z]+ { words++;chars+=strlen(yytext);}
\n        { chars++;lines++;}
.         { chars++;}
%%

int main(int argc,char *argv[]){
 if(argc > 1){
    if(!(yyin = fopen(argv[1],"r"))){
       perror(argv[1]);
       return (1);
    }//end if clause
   yylex();
   printf("Total lines: %d\n",lines);
   printf("Total words: %d\n",words);
   printf("Total characters: %d\n",chars);
 }//end of function main
 return 0;
}
