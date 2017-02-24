typedef enum token_types{
	BEGIN,END,READ,WRITE,ID,INTLITERAL,
	LPAREN,RPAREN,SEMICOLON,COMMA,ASSIGNOP,
	PLUSOP,MINUSOP,SCANEOF
}token;
extern token scanner(void);
/**------------------------------*/

#include <stdio.h>
#include <ctype.h>
extern char token_buffer[];
token scanner(void){
	int in_char,c;
	clear_buffer();
	if (feof(stdin))
	{/*Si termine de leer el archivo*/
		return SCANEOF;
	}
	while((in_char = getchar()) != EOF){
			/*(in_char = getchar()) es el do IMPLICITO del while !=EOF*/
		if(isspace(in_char)){
			continue;
		}
		else if (isalpha(in_char))
		{
			/* code to recognize identifiers*/
			buffer_char(in_char);
			for (c = getchar(); isalnum(c) || c == '_'; c = getchar()){
				buffer_char(c);
			}
			ungetc(c,stdin);
			//return ID;		
		}
		else if (isdigit(in_char)){
			buffer_char(in_char);
			for (c = getchar(); isdigit(c); c = getchar()){
				buffer_char(c);
			}
			ungetc(c,stdin);
			return INTLITERAL;
		}
		else if (in_char == '('){
			return LPAREN;
		}
		else if (in_char == ')'){
			return RPAREN;	
		}
		else if (in_char == ';'){
			return SEMICOLON;	
		}
		else if (in_char == ','){
			return COMMA;
		}
		else if (in_char == '+'){
			return PLUSOP;
		}
		else if (in_char == ':'){
			/*buscando el :=*/
			c = getchar();
			if (c == "=")
			{
				return ASSIGNOP;
			}
			else{
				ungetc(c,stdin);
				lexical_error(in_char);
			}
		}
		else if (in_char == '-'){
			/* Looking for a comment*/
			c = getchar();
			if (c == '-')
			{
				while((in_char = getchar()) != '\n');
			}
			else{
				ungetc(c,stdin);
				return MINUSOP;
			}
		}
		else{
			lexical_error(in_char);
		}	

	}
}

