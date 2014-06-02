# !/bin/bash

yacc -d float.y
lex float.l
gcc -o parser y.tab.c lex.yy.c
