%{
#include <stdio.h>
#include "util.h"
#include "errormsg.h"

int yylex(void); /* function prototype */

void yyerror(char *s)
{
 EM_error(EM_tokPos, "%s", s);
}
%}


%union {
	int pos;
	int ival;
	string sval;
	}

%token <sval> ID STRING
%token <ival> INT


%right ASSIGN
%left  AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE

%right UMINUS

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%token 
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

%start program

%%

program:	exp

exp:    lvalue
   |    stmt
   |    INT PLUS INT {printf("%d + %d\n",$1,$3); }
   |    INT MINUS INT {printf("%d - %d\n",$1,$3); }
   |




id:     ID {  printf("id %s\n", $1);}

lvalue: id
      | lvalue DOT id
      | lvalue LBRACK exp RBRACK {printf("%s\n", "ARRAY");}

//各种语句
stmt :  funccall
     |  assignstmt
     |  ifstmt
if : IF {printf("ifsssss\n");}
//if 语句
ifstmt : if exp THEN exp %prec LOWER_THAN_ELSE         { printf("if-then\n");}
      |  if exp THEN exp ELSE exp {printf("if-then-else\n");}


assignstmt : id ASSIGN exp { printf("assign stmt\n");}

//函数调用
funccall : id LPAREN  args RPAREN { printf("funccall\n" );} 

/*参数*/
args    : exp COMMA args
        | exp
	
