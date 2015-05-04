%{
#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "table.h"
#include "absyn.h"

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
	S_symbol sym;
	A_var var;
	
	A_exp exp;
	A_expList explist;
	
	A_dec dec;
	A_decList declist;
	A_efield	efield;
	A_efieldList efieldlist;
	A_ty ty;
	A_field field;
	A_fieldList fieldlist;
	A_fundec fundec;
	A_fundecList funclist;
	A_namety namety;
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

  /*非终结符类型*/


  
%start program

%%

program:	exp

exp:    lvalue	{printf("lvalue to exp\n");}
   |    stmt_exp
   |	array_create
   |	record_create
   |	cmp_exp
   |    math_exp
   |    bool_exp
   |    INT
   |	STRING		
   |	NIL
   |	expseq {/*(a;b;c)*/}
   |	BREAK

id:     ID {  printf("id %s\n", $1);}   
   
cmp_exp :   exp EQ exp	{ }
		   | exp NEQ exp
		   | exp GT exp
		   | exp LT exp
		   | exp GE exp
		   | exp LE exp

bool_exp:	exp AND exp
			|exp OR exp
 
			   
math_exp:	exp PLUS exp {printf(" + \n"); }
	   |    exp MINUS exp {printf(" - \n"); }
	   |	exp TIMES exp
	   |	exp DIVIDE exp
	   |	MINUS exp %prec UMINUS		   

  
//与 lvalue 有已经规约冲突
array_create: id LBRACK exp RBRACK OF exp

//记录定义
record_create: id LBRACE recordfields RBRACE

lvalue: id lvalue_ext		 

lvalue_ext: DOT id lvalue_ext
      |  LBRACK exp RBRACK lvalue_ext{printf("%s\n", "ARRAY");}
	  |

	  
//各种语句
stmt_exp :  funcall_exp
     |  assign_stmt_exp	 
     |  if_stmt_exp
	 |  let_stmt_exp
	 |  for_stmt_exp
	 |  while_stmt_exp

//函数调用
funcall_exp : id LPAREN  explist RPAREN { printf("funcall_exp\n" );}	 
	 
//赋值表达式	 
assign_stmt_exp : lvalue ASSIGN exp { printf("assign stmt\n");}	

//if 语句
if_stmt_exp : IF exp THEN exp %prec LOWER_THAN_ELSE         { printf("if-then\n");}
			|  IF exp THEN exp ELSE exp {printf("if-then-else\n");}

//let 语句	 
let_stmt_exp: LET decs IN explist END			
	 
for_stmt_exp: FOR id ASSIGN exp TO exp DO exp
	 
while_stmt_exp:WHILE exp DO exp    {printf("while stmt\n");}
	 
//(exp1;exp2;exp3)
expseq: LPAREN expseq_nonempty RPAREN
expseq_nonempty: exp
				| exp SEMICOLON expseq_nonempty
	 

//语句序列
// exp1,exp2
explist:exp
		| exp COMMA explist
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
	 

	
