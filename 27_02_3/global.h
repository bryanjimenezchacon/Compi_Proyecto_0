/*****************************************************************************************************************************
						   Compiladores e Intérpretes Gr 40
							     Proyecto 0

						Bryan Steve Jiménez Chacón (2014114175)
						Ariel Montero Monestel     (2014073164)
						Luis Rojas Alfaro          (2014054405)

								IS 2017

******************************************************************************************************************************/




#ifndef GLOBAL_H
#define GLOBAL_H
#endif

/*-------------------------------------------------GLOBAL DATA------------------------------------------------------------*/

/*ENUM OF TOKENS*/
typedef enum token_types{
	BEGIN,END,READ,WRITE,ID,INTLITERAL,LPAREN,RPAREN,SEMICOLON,COMMA,
	ASSIGNOP,PLUSOP,MINUSOP,SCANEOF,PIPE
} token;
/*------------------------------------------------------------------------------------------------------------------------*/
typedef enum {false=0, true=1} bool;
