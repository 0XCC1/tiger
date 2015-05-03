%{
#include <string.h>
#include "util.h"
//#include "tokens.h"
#include "y.tab.h"
#include "errormsg.h"

#define	OVER_MEM_ERR printf("%s (max_length: %d)", \
						"usage: string out of memrory!", MAX_LENGTH);\
					 	exit(1);
						
#define PINFO 0
#define PALLINFO 0
						
#if PINFO
	#if PALLINFO
		#define dbgprint(show,fmt,args...) printf(fmt,##args);
	#else
		#define dbgprint(show,fmt,args...) if(show==1){printf(fmt,##args);}else{}
	#endif
#else
	#define dbgprint(fmt,args...)
#endif

 						


#define MAX_LENGTH 512
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

//string 
static char* str_ptr;
static char str[MAX_LENGTH];
static char  ch;
static int remain;

void init_string(){
	memset(str,0,MAX_LENGTH);
	remain=MAX_LENGTH-1;
	str_ptr=str;
}

void end_string() {
	if (!remain) {
		OVER_MEM_ERR;
	}
	
	*str_ptr++ = '\0';
}

void append_char_to_string(char ch) {
	if (!remain) {
		OVER_MEM_ERR;
	}
	*str_ptr++ = ch;
	remain--;
}

%}


%x string
%x comment

id 		[A-Za-z][A-Za-z0-9_]*
integer [0-9]+
ws      [ \t]+
newline (\r\n|\n)


%%
{ws}	 				{adjust();  dbgprint(0,"ws\n"); continue;}
{newline}	 			{adjust(); dbgprint(1,"newline\n"); EM_newline(); continue;}
","	 					{adjust(); return COMMA;}
":" 					{adjust(); return COLON;}
";" 					{adjust(); return SEMICOLON;}
"("						{adjust(); return LPAREN;}
")"						{adjust(); return RPAREN;}
"["						{adjust(); printf("lex: [\n"); return LBRACK;}
"]" 					{adjust(); printf("lex: ]\n"); return RBRACK;}
"{"						{adjust(); return LBRACE;}
"}"						{adjust(); return RBRACE;}
"+"						{adjust(); dbgprint(0,"plus\n"); return PLUS;}
"-"						{adjust(); return MINUS;}
"."						{adjust(); return DOT;}
"/"						{adjust(); return DIVIDE;}
"*"						{adjust(); return TIMES;}
"="						{adjust(); printf("eq \n"); return EQ;}
"<>"					{adjust(); return NEQ;}
"<"						{adjust(); return LT;}
"<="					{adjust(); return LE;}
">"						{adjust(); return GT;}
">="					{adjust(); return GE;}
"&"						{adjust(); return AND;}
"|"						{adjust(); return OR;}
":="					{adjust(); return ASSIGN;}
for  	 				{adjust(); return FOR;}
{integer}	 			{adjust(); yylval.ival=atoi(yytext); dbgprint(0,"int %d\n",yylval.ival); return INT;}
while					{adjust(); return WHILE; }
to 						{adjust(); return TO;}
break 					{adjust(); return BREAK;}
let						{adjust(); return LET;}
in 						{adjust(); return IN;}
end 					{adjust(); return END;}
function 				{adjust(); dbgprint(0,"function\n"); return FUNCTION;}
var 					{adjust(); return VAR;}
type					{adjust(); return TYPE;}
array					{adjust(); return ARRAY;}
if						{adjust(); dbgprint(0,"ifffff\n");  return IF;}
then					{adjust(); return THEN;}
else					{adjust(); return ELSE;}
do 						{adjust(); return DO;}
of   					{adjust(); return OF;}
nil  					{adjust(); return NIL;}
{id}					{adjust(); yylval.sval = strdup(yytext);   dbgprint(1,"%s\n",yylval.sval);  return ID;}


\"					    {adjust(); printf("start string \n"); init_string(); BEGIN string;}
<string>{
	\" 					{
							adjust(); 
							end_string(); 
							yylval.sval = strdup(str);
							BEGIN (0); 
							printf("string: %s\n",yylval.sval);
							return STRING;
						}
	\\\"				{	
							adjust();
							append_char_to_string(*yytext);
							append_char_to_string(yytext[1]);
							//yylval.sval = strdup(str);
						}
	.					{ adjust(); printf("char: %c\n",*yytext); append_char_to_string(*yytext); }
}

"/*"						{/*printf("begin comment");*/ adjust();  BEGIN comment;}
<comment>{
"*/"				{ /*printf("out comment  %s  ",yytext);*/  BEGIN(0); continue;}
(.|\n)				{ adjust(); /*printf("%s",yytext);*/ continue;}
}



.	     				{adjust(); ;EM_error(EM_tokPos,"illegal token"); printf("%s\n",yytext);}
