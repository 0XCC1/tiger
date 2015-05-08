%{
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "tiger.tab.h"
#include "errormsg.h"

#define MAX_STRING_COUNT (1000) /* hope this is enouth */
int charPos=1;

int yywrap(void)
{
  charPos=1;
  return 1;
}

void adjust(void)
{
  EM_tokPos=charPos;
  charPos+=yyleng;
}

%}

/*Start conditions for comment and string processing*/
%x comment string

%%
         int cmt_level = 0; /*use to handle nested comment, must indented.*/
         char string_buf[MAX_STRING_COUNT]={0};
         char* sbuf_ptr;

" "	 {adjust(); continue;}
\t       {adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}

","	 {adjust(); return COMMA;}
":"      {adjust(); return COLON;}
";"      {adjust(); return SEMICOLON;}
"("      {adjust(); return LPAREN;}
")"      {adjust(); return RPAREN;}
"["      {adjust(); return LBRACK;}
"]"      {adjust(); return RBRACK;}
"{"      {adjust(); return LBRACE;}
"}"      {adjust(); return RBRACE;}
"."      {adjust(); return DOT;}
"+"      {adjust(); return PLUS;}
"-"      {adjust(); return MINUS;}
"*"      {adjust(); return TIMES;}
"/"      {adjust(); return DIVIDE;}
"="      {adjust(); return EQ;}
"<>"     {adjust(); return NEQ;}
"<"      {adjust(); return LT;}
"<="     {adjust(); return LE;}
">"      {adjust(); return GT;}
">="     {adjust(); return GE;}
"&"      {adjust(); return AND;}
"|"      {adjust(); return OR;}
":="     {adjust(); return ASSIGN;}

array    {adjust(); return ARRAY;}
if       {adjust(); return IF;}
then     {adjust(); return THEN;}
else     {adjust(); return ELSE;}
while    {adjust(); return WHILE;}
for  	 {adjust(); return FOR;}
to       {adjust(); return TO;}
do       {adjust(); return DO;}
let      {adjust(); return LET;}
in       {adjust(); return IN;}
end      {adjust(); return END;}
of       {adjust(); return OF;}
break    {adjust(); return BREAK;}
nil      {adjust(); return NIL;}
function {adjust(); return FUNCTION;}
var      {adjust(); return VAR;}
type     {adjust(); return TYPE;}
  /*build-in type*/
  /*int      {adjust(); return INT;}
    string   {adjust(); return STRING;}*/
  /*int*/
[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}

  /*id: remark by xdl2007: when connected with parser, lexer should return S_symbol type of ID*/
[a-zA-Z][a-zA-Z0-9_]* {adjust(); yylval.symbol=S_Symbol(String(yytext)); return ID;}

  /*nested comment*/
"/*"     {adjust(); cmt_level++; BEGIN(comment);}
<comment>"/"+"*" {adjust(); cmt_level++; /*for '//*' */}
<comment>[^*/\n]* {adjust(); continue; /*actually 'continue' is not neccessary */}
<comment>"\n" {adjust(); EM_newline(); continue;}
<comment>"*"+[^*/\n]* {adjust(); continue;}
<comment>"*"+"/" {adjust(); cmt_level--; if(cmt_level == 0) BEGIN(INITIAL);}
<comment>. {adjust(); continue;/*otherwise, must be the last*/}

  /*string*/
\"          {adjust(); sbuf_ptr=string_buf; BEGIN(string);}
<string>\"  {
  adjust(); 
  *sbuf_ptr='\0'; 
  yylval.sval=String(string_buf);
  BEGIN(INITIAL);
  return STRING;
}
<string>\\n {adjust(); *sbuf_ptr++='\n';}
<string>\\t {adjust(); *sbuf_ptr++='\t';}
<string>\\\" {adjust(); *sbuf_ptr++='"';}
  /*for \\ in source, why '\\\\' can't but (\\){2} works? */
<string>(\\){2} {adjust(); *sbuf_ptr++='\\';}
  /*\^c for any appropriate c (e.g \r,\a,\b,\f and so on.) */
<string>\\\^. {
  adjust(); 
  switch(yytext[2]){
  case 'a':
    *sbuf_ptr++='\a';
    break;
  case 'b':
    *sbuf_ptr++='\b';
    break;
  case 'r':
    *sbuf_ptr++='\r';
    break;
  case 't':
    *sbuf_ptr++='\t';
    break;
  case 'v':
    *sbuf_ptr++='\v';
    break;
  case 'f':
    *sbuf_ptr++='\f';
    break;
 case 'e':
    *sbuf_ptr++='\e';
    break;
  deault:
    EM_error(EM_tokPos,"illegal escape sequence");
    break;
  }
 }
  /*\ddd, which contains \0 */
<string>\\[0-9]{1,3} {
  adjust();
  int result;
  /*remember yytext[0] == '\',so plus 1 */
  (void)sscanf(yytext+1,"%d",&result);
  if(result > 0x7f || result < 0)
    {
      EM_error(EM_tokPos,"out of bound");
      exit(1);
    }
  *sbuf_ptr++=result;
 }
<string>[^\"\n\\]+ {
  adjust();
  char* yptr = yytext;
  while(*yptr)
    *sbuf_ptr++=*yptr++;
 }
  /*ignore \f---f\, actually the compiler thinks this is just one line*/
<string>\\[" "\n\t\f]+\\ {adjust(); continue;}

  /*remark by xdl2007: when connected with parser, remove 'exit(1)'*/
<<EOF>>  {if(cmt_level!=0) EM_error(EM_tokPos, "unclosed comment"); return EOF;/*exit(1);*/}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}

