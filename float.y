%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Float_Functions.h"

extern int least_precision;

int yylex();

#define NVARS 100
char *vars[NVARS]; double vals[NVARS]; int IsvalsDouble[NVARS]; int Isvals64[NVARS];
float var[NVARS];
Float Var[NVARS];

int nvars=0;

int IsTermDouble = 0;
int IsMuldivDouble = 0;
int IsExprDouble = 0;
int line_num = 2;

%}
%union { Float dval; int ivar; char *sval; }

%token <sval> FLOAT
%token <ivar> INT; 
%token <ivar> VARIABLE
%token <ivar> FLOAT32;
%token <ivar> FLOAT64;
%token <sval> PRECISION;
%type <dval> expr
%type <dval> muldiv
%type <dval> term


%%
wholeprogram
	: PRECISION INT '\n' {printf("Wanted Precision: %d\n", $2); least_precision = $2;} program	
	; 

program
	: {printf("\nProcessing Line %d Now...\n", line_num); line_num++;} statement program			{ }
	|
	;
statement
	: expr '\n'					{ } 
	| VARIABLE '=' expr '\n' 	{ Var[$1] = $3; } 
	;

expr
	: expr '+' muldiv { $$ = add($1, $3); printf("this is +\n"); printinfo($$);}// 
	| expr '-' muldiv { $$ = sub($1, $3); printf("this is -\n"); printinfo($$);}
	| muldiv { $$ = $1; }
	;
muldiv
	: muldiv '*' term { $$ = multiply($1, $3); printf("this is *\n"); printinfo($$);}
	| muldiv '/' term { }
	| term { $$ = $1; }
	;
term
	: '(' expr ')' { $$ = $2; }
	| VARIABLE {  $$ = Var[$1]; printinfo($$); }
	| FLOAT ':' INT { $$ = new_float(atof($1), $3); printinfo($$); }
	| FLOAT { $$ = new_float(atof($1), 7); printinfo($$); }
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
	//| FLOAT { $$ = new_float(atof($1), 7); printf("bbbbb\n");printinfo($$); }
}
int yyerror(char *s) {
    printf("%s\n", s);
}
int main(void)
{
	yyparse();
	return 0;
}