/*****************************************************************************************************************************
						   Compiladores e Intérpretes Gr 40
							     Proyecto 0

						Bryan Steve Jiménez Chacón (2014114175)
						Ariel Montero Monestel     (2014073164)
						Luis Rojas Alfaro          (2014054405)

								IS 2017

******************************************************************************************************************************/
#ifndef SCANNER_H
#define SCANNER_H
#endif

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include "global.h" /*Global variables*/

/*--Data definition-----------------------------------------------------------------------------------------------*/

int len, len_token_buffer, c, comment_char, filePos, charPos,len_tb;
FILE *file;
char token_buffer[5];
char begin_buffer[5] = "BEGIN";
char end_buffer[3] = "END";
char read_buffer[4] = "READ";
char write_buffer[5] = "WRITE";

/*--Function definition-------------------------------------------------------------------------------------------*/

extern token scanner(void); //It produces a representation of tokens

void read_file(char *input); // reads the input file

void close_file(); //closes the input file

void buffer_char(char c); //Stores the characters of a token
 
void clear_token_buffer(); //Cleans the buffer_token

token check_reserved(); //Checks if an ID is a token 

void lexical_error(); //Returns an error in the output file
