#ifndef GLOBAL_H
#define GLOBAL_H
#endif

/*--------------------------------------------GLOBAL DATA---------------------------------------*/

/*ENUM OF TOKENS*/
typedef enum token_types{
	BEGIN,END,READ,WRITE,ID,INTLITERAL,LPAREN,RPAREN,SEMICOLON,COMMA,
	ASSIGNOP,PLUSOP,MINUSOP,SCANEOF,PIPE
} token;
/*------------------------------------------------------------------------------------------------------------------------*/
typedef enum {false=0, true=1} bool;
