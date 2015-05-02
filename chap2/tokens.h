typedef union  {
	int pos;
	int ival;
	string sval;
	} YYSTYPE;
extern YYSTYPE yylval;


# define ID 257
# define STRING 258
//
# define INT 259
//,
# define COMMA 260
//:
# define COLON 261
//;
# define SEMICOLON 262
//(
# define LPAREN 263
//)
# define RPAREN 264
//[
# define LBRACK 265
//]
# define RBRACK 266
//{
# define LBRACE 267
//}
# define RBRACE 268
// .
# define DOT 269
//+
# define PLUS 270
// -
# define MINUS 271
// *
# define TIMES 272
// /
# define DIVIDE 273
// =
# define EQ 274
// <>
# define NEQ 275
// <
# define LT 276
// <=
# define LE 277
// >
# define GT 278
// >=
# define GE 279
// &
# define AND 280
// |
# define OR 281
// :=
# define ASSIGN 282
// array
# define ARRAY 283
// if
# define IF 284
//then
# define THEN 285
//else
# define ELSE 286
//while
# define WHILE 287
// for
# define FOR 288
//to
# define TO 289
//do 
# define DO 290
//let 
# define LET 291
// in
# define IN 292
//end 
# define END 293
//of
# define OF 294
//break
# define BREAK 295
// nil
# define NIL 296
// function
# define FUNCTION 297
// var
# define VAR 298
//type
# define TYPE 299

