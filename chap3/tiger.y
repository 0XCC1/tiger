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
%right DO OF
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
   |	comparestmt
   |    exp PLUS exp {printf(" + \n"); }
   |    exp MINUS exp {printf(" - \n"); }
   |	exp TIMES exp
   |	exp DIVIDE exp
   |	MINUS exp %prec UMINUS
   |    INT
   |	STRING		
   |	NIL
   |	expblock
   |	BREAK
   
   
comparestmt :   exp EQ exp
			   | exp NEQ exp
			   | exp GT exp
			   | exp LT exp
			   | exp GE exp
			   | exp LE exp
			   | exp AND exp
			   | exp OR exp 


//与 lvalue 有已经规约冲突
arraydefine: id LBRACK exp RBRACK OF exp

id:     ID {  printf("id %s\n", $1);}

lvalue: id		{printf("lvalue \n");}
      | lvalue DOT id
      | lvalue LBRACK exp RBRACK {printf("%s\n", "ARRAY");}

//各种语句
stmt :  funccall
     |  assignstmt	 
     |  ifstmt
	 |  letstmt
	 |  forstmt
	 |  whilestmt
	
whilestmt:WHILE exp DO exp    {printf("while stmt\n");}
	 
forstmt: FOR id ASSIGN exp TO exp DO exp
	 

//(exp1;exp2;exp3)
expblock: LPAREN expseq RPAREN
	 
//let 语句	 
letstmt: LET decs IN expseq END

//语句序列
// exp1;exp2
expseq:exp
	| exp SEMICOLON expseq
	|

//声明语句块
decs: dec decs
	|


dec : tydec
	| vardec 	{/*printf("declare a var\n");*/}
	| fundec

//函数定义
fundec:	FUNCTION id LPAREN tyfields RPAREN EQ exp {/*没有返回类型*/}
	|	FUNCTION id LPAREN tyfields RPAREN COLON id EQ exp {/*有返回类型*/}
	
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
	
//id1 :int, id2 string
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
		|
	
