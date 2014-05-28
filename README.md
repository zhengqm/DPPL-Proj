
Freesa － A Float DSL With Statical Analysis
=======================================
----------------------------------------------

# How to Use?

To get Freesa source files, open your `shell` and type

	git clone https://github.com/zhengqm/DPPL-Proj.git

To compile the files, type

	yacc -d float.y
	lex float.l
	gcc -o parser y.tab.c lex.yy.c
	
Then you get your `parser` for `Freesa`. 

---------------------------------------------

# What is Freesa Language?

First, let's see the **lexical rules** for `Freesa`.

	[ \t]			; 										//ignore tab and whitespaces
	[0-9]+\.[0-9]+	{yylval.sval = yytext; return FLOAT;} 	//float numbers
	[-+()=/*\n]		{return *yytext;} 						//operators
	[a-z]			{yylval.ivar = varindex(yytext); return VARIABLE;} //varaible names
	.				{;} 									//anything else.
	
After that, we can look into `Freesa`'s **semantic grammars**.

	program
		: statement program	
		|
		;
	statement
		: expr '\n'					
		| VARIABLE '=' expr '\n' 	
		;
	expr
		: expr '+' muldiv 
		| expr '-' muldiv 
		| muldiv 
		;
	muldiv
		: muldiv '*' term 
		| muldiv '/' term 
		| term 
		;
	term
		: '(' expr ')' 
		| VARIABLE 
		| FLOAT 
		;

As we can see, `Freesa` supports language that:

- float numbers and float variables with their primitive calculation.
- variables should only in **one-letter-lowercase** letter.
- parentheses are also accepted.


------------------------------------


# An Example

Let's see an eample, it is also the file `input.fl` in our repository.

	1.2
	a = 1.2 * 1.2
	b = 1.0
	c = a - b
	d = 180.0 * a
	e = (b + c) * a
	e

If we run them in our `parser`, the output is like

	...
	Processing Line 2 Now...

	value: 1.440000e+00
	eps[0]: 0.000000e+00
	eps[1]: 7.152558e-08
	eps[2]: 7.152558e-08
	eps[3]: 5.722046e-08
	epshi: 3.552714e-15
	Predicted Max Absolute Error: 2.002716e-07
	Predicted Max Relative Error: 1.390775e-07
	...
	
This is the result of the second line. As we can see, `Freesa` will detect possible absolute errors and relative errors. We believe it's quite useful in many **float precision analysis**.

