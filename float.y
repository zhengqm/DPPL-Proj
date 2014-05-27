%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Float_Functions.h"
int yylex();

#define NVARS 100
char *vars[NVARS]; double vals[NVARS]; int IsvalsDouble[NVARS]; int Isvals64[NVARS];
float var[NVARS];
Float Var[NVARS];

int nvars=0;

int IsTermDouble = 0;
int IsMuldivDouble = 0;
int IsExprDouble = 0;

%}
%union { Float dval; int ivar; char *sval; }

%token <sval> FLOAT
%token <dval> INT; 
%token <ivar> VARIABLE
%token <ivar> FLOAT32;
%token <ivar> FLOAT64;
%type <dval> expr
%type <dval> muldiv
%type <dval> term


%%
program
	: statement program
	|
	;
statement
	: expr '\n'					{ } 
	| VARIABLE '=' expr '\n' 	{ Var[$1] = $3; printinfo(Var[$1]); } 
	;

expr
	: expr '+' muldiv { $$ = add($1, $3); printinfo($$);}// 
	| expr '-' muldiv { $$ = sub($1, $3); printinfo($$);}
	| muldiv { $$ = $1; }
	;
muldiv
	: muldiv '*' term { $$ = multiply($1, $3); printinfo($$);}
	| muldiv '/' term { }
	| term { $$ = $1; }
	;
term
	: '(' expr ')' { $$ = $2; }
	| VARIABLE {  $$ = Var[$1]; }
	| FLOAT { $$ = new_float(atof($1)); }
;
%%


int varindex(char *varname)
{
int i;
for (i=0; i<nvars; i++)
if (strcmp(varname,vars[i])==0)
return i;
vars[nvars] = strdup(varname);
return nvars++;
}
int yyerror(char *s) {
    printf("%s\n", s);
}
int main(void)
{
yyparse();
return 0;
}