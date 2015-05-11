%{
#include <stdio.h>
#include "stdlib.h"
#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "absyn.h"
#include "prabsyn.h"
#include "parse.h"
#include "types.h"
#include "translate.h"
#include "semant.h"


int yylex(void); /* function prototype */

A_exp absyn_root;

void yyerror(char *s)
{
  EM_error(EM_tokPos, "%s", s);
}
%}

%union {
	A_pos pos;
	int ival;
	string sval;
        S_symbol symbol;
        A_var var;
        A_exp exp;
        A_expList seq; //args for function call, seq for expseq
        A_oper oper;
        A_efield efield;
        A_efieldList efields;
        A_dec dec;
        A_namety namety;
        A_fundec fundec;
        A_ty ty;
        A_decList decs;
        A_fundecList function;
        A_nametyList type;
        A_field field;
        A_fieldList record;
}

%token <sval> STRING
%token <ival> INT

%nonassoc ASSIGN ID
%left AND OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS
%right THEN ELSE DO OF
%nonassoc LBRACK

// remark by xdl2007: for tydec and fundec
// notes: the '$default' in tiger.output is the 'default' action to do if no
// token-specific action applies. This is completely independent of the default
// resolution for a conflict (two different defaults fow two different situlation)
// ref: Chris Dodd from stackoverflow
%expect 2

%token
  COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK 
  LBRACE RBRACE DOT 
  PLUS MINUS TIMES DIVIDE EQ NEQ LT LE GT GE
  AND OR ASSIGN
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF 
  BREAK NIL
  FUNCTION VAR TYPE 

%type <exp> exp program funcall arith_exp comp_exp bool_exp record_create array_create
%type <var> lvalue
%type <symbol> ID
%type <seq> expseq expseq_nonempty explist explist_nonempty
%type <decs> decs
%type <dec> dec tydeclist vardec fundeclist
%type <ty> ty
%type <namety> tydec
%type <fundec> fundec
%type <record> tyfields tyfields_nonempty
%type <field> tyfield
%type <efields> record_create_list record_create_list_nonempty
%type <efield> record_create_field

%start program

%%

program:	exp  { absyn_root = $1; }
;

// remark by xdl2007: the semantic actions are used for building an
// abstract parse tree and store the whole syntax tree in memory.
exp:     NIL                             { $$ = A_NilExp(@1.first_column); }
       | INT                             { $$ = A_IntExp(@1.first_column,$1); }
       | STRING                          { $$ = A_StringExp(@1.first_column, $1); }
       | BREAK                           { $$ = A_BreakExp(@1.first_column); }
       | lvalue                          { $$ = A_VarExp(@1.first_column, $1); }
       | arith_exp                       { $$ = $1; }
       | comp_exp                        { $$ = $1; }
       | bool_exp                        { $$ = $1; }
       | lvalue ASSIGN exp               { $$ = A_AssignExp(@1.first_column,$1,$3); }
       | MINUS exp %prec UMINUS          { $$ = A_OpExp(@2.first_column, A_minusOp, A_IntExp(@2.first_column,0),$2); } // 0 - exp 
       | LPAREN expseq RPAREN            { $$ = A_SeqExp(@2.first_column,$2); }
       | IF exp THEN exp ELSE exp        { $$ = A_IfExp(@2.first_column,$2,$4,$6); }
       | IF exp THEN exp                 { $$ = A_IfExp(@2.first_column,$2,$4,NULL); }
       | WHILE exp DO exp                { $$ = A_WhileExp(@2.first_column,$2,$4); }
       | FOR ID ASSIGN exp TO exp DO exp { $$ = A_ForExp(@2.first_column,$2,$4,$6,$8); }
       | LET decs IN expseq END          { $$ = A_LetExp(@2.first_column,$2,A_SeqExp(@4.first_column,$4)); }
       | funcall                         { $$ = $1; }
       | record_create                   { $$ = $1; }
       | array_create                    { $$ = $1; }
;

lvalue:  ID                              { $$ = A_SimpleVar(@1.first_column, $1); }
       | ID LBRACK exp RBRACK            { $$ = A_SubscriptVar(@1.first_column, A_SimpleVar(@1.first_column,$1),$3); } /*reduntant rule*/
       | lvalue DOT ID                   { $$ = A_FieldVar(@1.first_column, $1, $3); }
       | lvalue LBRACK exp RBRACK        { $$ = A_SubscriptVar(@1.first_column, $1, $3); }
;

/*
exp1;exp2;exp3
*/
expseq: /*emtpy*/                        { $$ = NULL;}
       | expseq_nonempty                 { $$ = $1; }
;

expseq_nonempty: exp                     { $$ = A_ExpList($1,NULL);  }
       | exp SEMICOLON expseq_nonempty   { $$ = A_ExpList($1,$3);    }
;
/*
fun(exp1,exp2,exp3)
*/
funcall: ID LPAREN explist RPAREN        { $$ = A_CallExp(@1.first_column,$1,$3); }
;

explist: /*empty*/                       { $$ = NULL;}
      | explist_nonempty                 { $$ = $1; }
;

/*
exp1,exp2,exp3
*/
explist_nonempty: exp                    { $$ = A_ExpList($1,NULL); }
      | exp COMMA explist_nonempty       { $$ = A_ExpList($1,$3); }
;

arith_exp: exp PLUS exp                  { $$ = A_OpExp(@1.first_column,A_plusOp,$1,$3);   }
      | exp MINUS exp                    { $$ = A_OpExp(@1.first_column,A_minusOp,$1,$3);  }
      | exp TIMES exp                    { $$ = A_OpExp(@1.first_column,A_timesOp,$1,$3);  }
      | exp DIVIDE exp                   { $$ = A_OpExp(@1.first_column,A_divideOp,$1,$3); }
;

comp_exp:exp EQ exp                      { $$ = A_OpExp(@1.first_column,A_eqOp,$1,$3);  }
       | exp NEQ exp                     { $$ = A_OpExp(@1.first_column,A_neqOp,$1,$3); }
       | exp LT exp                      { $$ = A_OpExp(@1.first_column,A_ltOp,$1,$3);  }
       | exp LE exp                      { $$ = A_OpExp(@1.first_column,A_leOp,$1,$3);  }
       | exp GT exp                      { $$ = A_OpExp(@1.first_column,A_gtOp,$1,$3);  }
       | exp GE exp                      { $$ = A_OpExp(@1.first_column,A_geOp,$1,$3);  }
;

 /* remark by xdl2007:  change to if expression*/
bool_exp:exp AND exp                     { $$ = A_IfExp(@1.first_column,$1,$3,A_IntExp(@1.first_column,0)); }
       | exp OR exp                      { $$ = A_IfExp(@1.first_column,$1,A_IntExp(@1.first_column,1),$3); }
;

decs:  /*empty*/                         { $$ = NULL;}
       | dec decs                        { $$ = A_DecList($1,$2); }
;

dec:     tydeclist                       { $$ = $1; }
       | vardec                          { $$ = $1; }
       | fundeclist                      { $$ = $1; }
;

//this has some problems, actually it's not context-free grammar, but how to catch the time of A_TypeDec ?

tydeclist: tydec                         { $$ = A_TypeDec(@1.first_column,A_NametyList($1,NULL));}
      | tydec tydeclist                  { $$ = A_TypeDec(@1.first_column,A_NametyList($1,$2->u.type));  }
;
/*
type ty1 = int
type ty2 = {id1:ty1,id2:ty2}
type ty3 = array of int
*/
tydec:  TYPE ID EQ ty                    { $$ =  A_Namety($2,$4); }
;

ty:     ID                               { $$ = A_NameTy(@1.first_column,$1); }
      //{id:type(id2), id2:type(id3)}
      | LBRACE tyfields RBRACE           { $$ = A_RecordTy(@2.first_column,$2); }
      | ARRAY OF ID                      { $$ = A_ArrayTy(@3.first_column,$3); }
;

/*There may have a problem.*/
tyfields: /*empty*/                      { $$ = NULL;}
      | tyfields_nonempty                { $$ = $1; }
;
/*
id : id
*/
tyfield: ID COLON ID                     { $$ = A_Field(@1.first_column,$1,$3); }
;
//id1 : ty1 , id2 : ty2
tyfields_nonempty:   tyfield             { $$ = A_FieldList($1,NULL); }
      | tyfield COMMA tyfields_nonempty  { $$ = A_FieldList($1,$3); }
;

vardec:  VAR ID ASSIGN exp               { $$ = A_VarDec(@2.first_column,$2,NULL,$4); }
       | VAR ID COLON ID ASSIGN exp      { $$ = A_VarDec(@2.first_column,$2,$4,$6);   }
;

//remark by xingdl2007: this is not context-free grammar actually.
fundeclist: fundec { $$ = A_FunctionDec(@1.first_column,A_FundecList($1,NULL)); }
      | fundec fundeclist { $$ = A_FunctionDec(@1.first_column,A_FundecList($1,$2->u.function)); }
;
fundec:  FUNCTION ID LPAREN tyfields RPAREN EQ exp             { $$ = A_Fundec(@2.first_column,$2,$4,NULL,$7); }
      | FUNCTION ID LPAREN tyfields RPAREN COLON ID EQ exp     { $$ = A_Fundec(@2.first_column,$2,$4,$7,$9);   }
;
/*
{id1=exp1,id2=exp2}
*/
record_create: ID LBRACE record_create_list RBRACE             { $$ = A_RecordExp(@1.first_column,$1,$3); }
;

record_create_list: /*empty*/                                  { $$ = NULL;}
      | record_create_list_nonempty                            { $$ = $1; }
;
/*
id=exp
*/
record_create_field: ID EQ exp                                 { $$ = A_Efield($1,$3); }
;
//id1=exp1,id2=exp2 ===> efieldlist
record_create_list_nonempty: record_create_field               { $$ = A_EfieldList($1,NULL); }
      | record_create_field COMMA record_create_list_nonempty  { $$ = A_EfieldList($1,$3); }
;
//id1[exp1] of exp2
array_create: ID LBRACK exp RBRACK OF exp                      { $$ = A_ArrayExp(@1.first_column, $1,$3,$6); }
;

%%

int main(int argc, char*argv[])
{
  // input filename
  A_exp tree = NULL;

  FILE *astfile;
  FILE *irfile;
  int indent = 10;
  if(argc != 4)
  {
    fprintf(stderr,"usage: parser src.tig ast.txt ir.txt \n");
    exit(1);
  }

  // remark by xingdl2007: need -t(--debug) falg when invoke bison
  // -d (--defines which will produce tiger.tab.[ch] file)
  // -t (--debug which will produce detail information when parsing)
  yydebug = 1;
  
  printf("start.\n");
  
  tree = parse(argv[1]);
  printf("stage 1.\n");
  astfile = fopen(argv[2],"w+");
  irfile = fopen(argv[3],"w+");

  if(tree && astfile)
  {
    printf("stage 2.\n");
    pr_exp(astfile,tree,indent);
  }

  printf(" ast generated .\n");

  printf("\n--------------- start semantic phase---------------\n");
  F_fragList f1= SEM_transProg(tree);
  
  printf("-----------------------IR-------------------------------\n");
  print_frag(f1, irfile);
  return 0;
}

