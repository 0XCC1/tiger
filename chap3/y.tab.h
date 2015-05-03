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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 258,
    STRING = 259,
    INT = 260,
    ASSIGN = 261,
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
    DO = 274,
    OF = 275,
    UMINUS = 276,
    LOWER_THAN_ELSE = 277,
    ELSE = 278,
    COMMA = 279,
    COLON = 280,
    SEMICOLON = 281,
    LPAREN = 282,
    RPAREN = 283,
    LBRACK = 284,
    RBRACK = 285,
    LBRACE = 286,
    RBRACE = 287,
    DOT = 288,
    ARRAY = 289,
    IF = 290,
    THEN = 291,
    WHILE = 292,
    FOR = 293,
    TO = 294,
    LET = 295,
    IN = 296,
    END = 297,
    BREAK = 298,
    NIL = 299,
    FUNCTION = 300,
    VAR = 301,
    TYPE = 302
  };
#endif
/* Tokens.  */
#define ID 258
#define STRING 259
#define INT 260
#define ASSIGN 261
#define AND 262
#define OR 263
#define EQ 264
#define NEQ 265
#define LT 266
#define LE 267
#define GT 268
#define GE 269
#define PLUS 270
#define MINUS 271
#define TIMES 272
#define DIVIDE 273
#define DO 274
#define OF 275
#define UMINUS 276
#define LOWER_THAN_ELSE 277
#define ELSE 278
#define COMMA 279
#define COLON 280
#define SEMICOLON 281
#define LPAREN 282
#define RPAREN 283
#define LBRACK 284
#define RBRACK 285
#define LBRACE 286
#define RBRACE 287
#define DOT 288
#define ARRAY 289
#define IF 290
#define THEN 291
#define WHILE 292
#define FOR 293
#define TO 294
#define LET 295
#define IN 296
#define END 297
#define BREAK 298
#define NIL 299
#define FUNCTION 300
#define VAR 301
#define TYPE 302

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "tiger.y" /* yacc.c:1909  */

	int pos;
	int ival;
	string sval;
	

#line 155 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
