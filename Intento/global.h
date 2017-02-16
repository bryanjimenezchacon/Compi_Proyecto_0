#ifndef GLOBAL_H
#define GLOBAL_H
#endif

/*--------------------------------------------Contiene todos los datos globales---------------------------------------*/

/*Representa el conjunto de tipos de tokens*/
typedef enum token_types{
	BEGIN,END,READ,WRITE,ID,INTLITERAL,LPAREN,RPAREN,SEMICOLON,COMMA,
	ASSIGNOP,PLUSOP,MINUSOP,SCANEOF
} token;
/*------------------------------------------------------------------------------------------------------------------------*/
typedef enum {false=0, true=1} bool;
