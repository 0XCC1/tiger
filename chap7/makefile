parser: parse.o prabsyn.o tiger.tab.o lex.yy.o errormsg.o util.o symbol.o absyn.o table.o types.o env.o semant.o temp.o translate.o uframe.o tree.o printtree.o  
	cc -g parse.o tiger.tab.o lex.yy.o prabsyn.o table.o errormsg.o symbol.o absyn.o util.o  types.o env.o  semant.o temp.o translate.o uframe.o  tree.o printtree.o -o IR
	rm -f *.o
	
parse.o: parse.c errormsg.h util.h
	cc -g -c parse.c

absyn.o: absyn.c absyn.h
	cc -g -c absyn.c

table.o: table.c table.h
	cc -g -c table.c

prabsyn.o: prabsyn.c prabsyn.h
	cc -g -c prabsyn.c

symbol.o: symbol.c symbol.h
	cc -g -c symbol.c

tiger.tab.o: tiger.tab.c
	cc -g -c tiger.tab.c

tiger.tab.c: tiger.y
	bison -dt tiger.y

tiger.tab.h: tiger.tab.c
	echo "tiger.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c

lex.yy.o: lex.yy.c tiger.tab.h errormsg.h util.h
	cc -g -c lex.yy.c

lex.yy.c: tiger.lex
	lex tiger.lex

util.o: util.c util.h
	cc -g -c util.c

types.o: types.c types.h
	cc -g -c types.c

env.o:env.h env.c
	cc -g -c env.c 

semant.o:semant.h semant.c
	cc -g -c semant.c
temp.o:temp.h temp.c
	cc -g -c temp.c

translate.o: translate.c translate.h
	cc -g -c translate.c

uframe.o:frame.h uframe.c
	cc -g -c uframe.c

tree.o: tree.h tree.c
	cc -g -c tree.c
printtree.o: printtree.h printtree.c
	cc -g -c printtree.c

 
clean: 
	rm -f tiger.tab.* *.o parser *.output
