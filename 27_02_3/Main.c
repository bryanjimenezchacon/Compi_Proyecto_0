/*****************************************************************************************************************************
						   Compiladores e Intérpretes Gr 40
							     Proyecto 0

						Bryan Steve Jiménez Chacón (2014114175)
						Ariel Montero Monestel     (2014073164)
						Luis Rojas Alfaro          (2014054405)

								IS 2017

******************************************************************************************************************************/

#include <stdio.h>
#include <string.h> /* memset */
#include <stdlib.h>
#include "scanner.c"
#include "parser.c"
#include "translator.c"


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

	translate();//Translates to ARM
}
