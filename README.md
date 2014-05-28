#Float Language
##For Static Analysis of Floating Point Accuracy

###Usage
	yacc -d float.y
	lex float.l
	gcc -o parser lex.yy.c y.tab.c
	./parser < input.fl
	
###Enviroment
OSX 10.9.2
