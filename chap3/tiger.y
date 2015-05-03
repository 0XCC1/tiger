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
   |	arraydefine
   |	record
   |    exp PLUS exp {printf(" + \n"); }
   |    exp MINUS exp {printf(" - \n"); }
   |    INT
   |	STRING


//与 lvalue 有已经规约冲突
arraydefine: id LBRACK exp RBRACK OF exp

id:     ID {  printf("id %s\n", $1);}

lvalue: id
      | lvalue DOT id
      | lvalue LBRACK exp RBRACK {printf("%s\n", "ARRAY");}

//各种语句
stmt :  funccall
     |  assignstmt
     |  ifstmt
	 |  letstmt

//let 语句	 
letstmt: LET decs IN expseq END

//语句序列
expseq:exp
	| exp SEMICOLON expseq
	|

//声明语句块
decs: dec decs
	|


dec : tydec
	| vardec 	{/*printf("declare a var\n");*/}
	//| fundec

//记录定义
record: id LBRACE recordfields RBRACE

recordfields: id EQ exp
			| id EQ exp COMMA recordfields
	
//变量声明
vardec: VAR ID ASSIGN exp
		| VAR id COLON id ASSIGN exp	
	
//类型声明	
tydec: TYPE id EQ ty

ty:	id
	| LBRACE tyfields RBRACE
	| ARRAY OF id 		{ printf("declare array type\n");}
	
tyfields :
		|id COLON id
		|id COLON id COMMA tyfields
	 
	 
//if 语句
ifstmt : IF exp THEN exp %prec LOWER_THAN_ELSE         { printf("if-then\n");}
      |  IF exp THEN exp ELSE exp {printf("if-then-else\n");}


assignstmt : lvalue ASSIGN exp { printf("assign stmt\n");}

//函数调用
funccall : id LPAREN  args RPAREN { printf("funccall\n" );} 

/*参数*/
args    : exp COMMA args
        | exp
	
