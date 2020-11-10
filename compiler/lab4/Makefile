.PHONY:lc,lcc,clean
lc:
	flex sysy.l
	gcc lex.yy.c -o lc.out
	./lc.out
lcc:
	flex -+ work5.l
	g++ lex.yy.cc -o lcc.out
	./lcc.out
