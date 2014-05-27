%{
	#include <stdlib.h>
	#include <stdio.h>
	#include "myFloat.h"
	#include "y.tab.h"
	int varindex(char * var);
%}

%%

[ \t]			;

[0-9]+			{yylval.sval = yytext; return INT;}

[0-9]+\.[0-9]+	{yylval.sval = yytext; return FLOAT;}

[-+()=/*\n]		{return *yytext;}

[a-z]			{yylval.ivar = varindex(yytext); return VARIABLE;}

.				{;}

%%

int yywrap(void) {
	return 1;
}