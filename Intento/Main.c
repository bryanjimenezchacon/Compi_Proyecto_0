#include <stdio.h>
#include <string.h> /* memset */
#include <stdlib.h>
#include "scanner.c"
#include "parser.c"
#include "translator.c"

// HOLAAAA
int main( int argc, char *argv[] )
{
	int i;
	read_file(argv[1]);
	open_outputFile();

	// Read Tokens
	nextev_token = scanner(); //Initialize
	system_goal();
	close_file();
	close_outputFile();

	translate();
}