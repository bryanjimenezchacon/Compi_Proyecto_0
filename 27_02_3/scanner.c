#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "scanner.h"

/*

READ FILE

It enable the program to use a file
The form to use this function is:

./Main "name_of_file"

*/

void read_file(char *input){
	file = fopen( input, "r" );
	if (file){
		len = ftell(file);
	}
	else{
		printf("Could'nt open file\n");
		exit(0);
	}
	
}


void close_file(){
	fclose(file);
}

void buffer_char(char c){
	token_buffer[charPos++] = c;	
}

void clear_token_buffer(){
	memset(token_buffer, 0, 5);
	charPos = 0;
}


/*

CHECK RESERVED

This function allows to know if the words are reserved words.  

This action allows the definition of the Program's Scope by giving meaning to the 
BEGIN, END clausules. Besides recognizes WRITE, READ commands. 


*/

token check_reserved(){
	int letter, c;
	bool reserved;
	//Checks if there is a token or an ID in the buffer_token 
	for(letter= 0; letter < len_tb; letter++){
		if ( 'B'== toupper(token_buffer[letter])){
			reserved = true;
			for(c = 0; c < 5; c++){//looking for a BEGIN or an ID
				if (begin_buffer[c] != toupper(token_buffer[letter++])){
					reserved = false;
					break;
				}
			}
			if(reserved == true){
				return BEGIN;
			}else{
				return ID;
			}

			break;
		}else if ('E' == toupper(token_buffer[letter])){
			reserved = true;
			for(c = 0; c < 3; c++){ //Looking for a END or an ID
				if (end_buffer[c] != toupper(token_buffer[letter++])){
					reserved = false;
					break;
				}
			}
			if(reserved == true){
				return END;
			}else{
				return ID;
			}
			break;
		}else if ('W' == toupper(token_buffer[letter])){
			reserved = true;
			for(c = 0; c < 5; c++){ //Looking for a WRITE or an ID
				if (write_buffer[c] != toupper(token_buffer[letter++])){
					reserved = false;
					break;
				}
			}
			if(reserved == true){
				return WRITE;
			}else{
				return ID;
			}
			break;
		}else if ('R' == toupper(token_buffer[letter])){
			reserved = true;
			for(c = 0; c < 4; c++){//Looking for a READ or an ID
				if (read_buffer[c] != toupper(token_buffer[letter++])){
					reserved = false;
					break;
				}
			}
			if(reserved == true){
				return READ;
			}else{
				return ID;
			}
			break;
		}else{
			return ID;
		}
	}
}


/*

SCANNER

This main function allows the program to understand a given text that works as a 
MICRO Language.

It defines the functions that the compiler must perform to achieve its goals. Translating
all the language into actions that were declared into the GLOBAL_H file

*/



token scanner(void){
	int in_char, i;
	clear_token_buffer();

	if (feof(file)){
		return SCANEOF;
	}
		
	len_tb = 0;//buffer's size. Used only if isalpha==1
	while ((in_char = getc(file)) != EOF){ //while there is not a end of file 
		if(isspace(in_char)){
			continue;
		} 

		// If it's an Alphabetical letter 
		else if (isalpha(in_char)){
			
			buffer_char(in_char);
			len_tb++;


			for (c = getc(file); isalnum(c) || c == '_'; c = getc(file)){
				buffer_char(c);
				len_tb++;
			}
			ungetc(c, file);// moves backward one position.
			return check_reserved();
		}
		else if (isdigit(in_char)){
			buffer_char(in_char);
			for (c = getc(file); isdigit(c); c = getc(file)){
				buffer_char(c);
			}
			ungetc(c, file);
			return INTLITERAL;
		}
		else if(in_char == '('){
			return LPAREN;
		
		}else if(in_char == ')'){
			return RPAREN;

		}else if(in_char == ';'){
			return SEMICOLON;

		}else if(in_char == ','){
			return COMMA;

		}else if(in_char == '|'){
			return PIPE;

		}else if(in_char == '+'){
			return PLUSOP;

		}else if (in_char == ':'){
			//Looking for ':='
			c = getc(file);
			if(c == '='){
				return ASSIGNOP;
			}else{
				lexical_error(in_char);
				break;
			}

		}else if(in_char == '-'){
			//Looking for a comment or a Minus Op 
			c = getc(file);
			if (c == '-'){
				do{
					in_char = getc(file);
				
					
				}while (in_char != '\n'); 
				

			}else{
				ungetc(c, file);
				return MINUSOP;
			}
		}else{
			lexical_error(in_char);
			break;
		}

	}
		
}


/*

LEXICAL ERROR

Shows if an error has ocurred; so it gives a warning to the user.

*/

void lexical_error(int character){
	printf("Exception in Function Scanner Error Type: Lexical Error \n\t at ReadInput(BadInput.MethLAB:666) \n\t at main(StupidInput.MethLAB:666) %d\n", character);
}
