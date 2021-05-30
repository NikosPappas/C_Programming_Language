%option noyywrap
/***declaring counters***/
%{
  int chars=0;
  int words=0;
  int lines=0;

  int total_characters=0;
  int total_words=0;
  int total_lines=0;
%}

/***rule definitions***/
%%
[a-zA-Z]+ { words++; chars+=strlen(yytext); }
\n        { chars++; lines++;}
.         { chars++; }
%%

/***user code section***/
int main(int argc,char *argv[]){
 int i;//for loop index
 if (argc < 2 ){//just read stdin
  yylex();
  printf("Lines: %d\nWords: %d\nChars: %d\n",lines,words,chars);
  return 0;
 }//end if clause
 for(i=1;i<argc;i++){
   FILE *f=fopen(argv[i],"r");
   if(!f){
    perror(argv[i]);
    return (1);
   }
  printf("Filename: %s\n",argv[i]);
  yyrestart(f);
  yylex();
  fclose(f);
  printf("Lines %d \nWords: %d\nCharacters: %d\n",lines,words,chars);
  total_characters+=chars;
  total_words+=words;
  total_lines+=lines;
 }
 if(argc > 1){
   printf("Total characters: %d\nTotal words: %d\nTotal lines:%d\n",total_characters,total_words,total_lines);
 }
 return 0;
}//end of function main
