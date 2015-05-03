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
    UMINUS = 274,
    LOWER_THAN_ELSE = 275,
    ELSE = 276,
    COMMA = 277,
    COLON = 278,
    SEMICOLON = 279,
    LPAREN = 280,
    RPAREN = 281,
    LBRACK = 282,
    RBRACK = 283,
    LBRACE = 284,
    RBRACE = 285,
    DOT = 286,
    ARRAY = 287,
    IF = 288,
    THEN = 289,
    WHILE = 290,
    FOR = 291,
    TO = 292,
    DO = 293,
    LET = 294,
    IN = 295,
    END = 296,
    OF = 297,
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
#define UMINUS 274
#define LOWER_THAN_ELSE 275
#define ELSE 276
#define COMMA 277
#define COLON 278
#define SEMICOLON 279
#define LPAREN 280
#define RPAREN 281
#define LBRACK 282
#define RBRACK 283
#define LBRACE 284
#define RBRACE 285
#define DOT 286
#define ARRAY 287
#define IF 288
#define THEN 289
#define WHILE 290
#define FOR 291
#define TO 292
#define DO 293
#define LET 294
#define IN 295
#define END 296
#define OF 297
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
