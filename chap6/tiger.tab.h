/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_TIGER_TAB_H_INCLUDED
# define YY_YY_TIGER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    INT = 259,
    ASSIGN = 260,
    ID = 261,
    AND = 262,
    OR = 263,
    EQ = 264,
    NEQ = 265,
    LT = 266,
    LE = 267,
    GT = 268,
    GE = 269,
    PLUS = 270,
    MINUS = 271,
    TIMES = 272,
    DIVIDE = 273,
    UMINUS = 274,
    THEN = 275,
    ELSE = 276,
    DO = 277,
    OF = 278,
    LBRACK = 279,
    COMMA = 280,
    COLON = 281,
    SEMICOLON = 282,
    LPAREN = 283,
    RPAREN = 284,
    RBRACK = 285,
    LBRACE = 286,
    RBRACE = 287,
    DOT = 288,
    ARRAY = 289,
    IF = 290,
    WHILE = 291,
    FOR = 292,
    TO = 293,
    LET = 294,
    IN = 295,
    END = 296,
    BREAK = 297,
    NIL = 298,
    FUNCTION = 299,
    VAR = 300,
    TYPE = 301
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 24 "tiger.y" /* yacc.c:1909  */

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

#line 123 "tiger.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_TIGER_TAB_H_INCLUDED  */
