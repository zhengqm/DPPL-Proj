%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "myFloat.h"
	#include "y.tab.h"
	int varindex(char * var);
%}

%%

[ \t]			; //ignore tab and whitespaces

[0-9]+\.[0-9]+	{yylval.sval = yytext; return FLOAT;} //float numbers

[-+()=*\n]		{return *yytext;} //operators

[a-z]			{yylval.ivar = varindex(yytext); return VARIABLE;} //varaible names

.				{;} //anything else.

%%

int yywrap(void) {
	return 1;
}