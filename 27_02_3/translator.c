/*****************************************************************************************************************************
						   Compiladores e Intérpretes Gr 40
							     Proyecto 0

						Bryan Steve Jiménez Chacón (2014114175)
						Ariel Montero Monestel     (2014073164)
						Luis Rojas Alfaro          (2014054405)

								IS 2017

******************************************************************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "translator.h"


void translate(){    
    
    // Decode Assembly Language
    translated_file = fopen("output_file.txt", "r" );
    
    // Destination file
    rasp_file = fopen ("rasp_file.s","w+");

    // Files that stores and recives code
    data_file = tmpfile();
    code_file = tmpfile();

    // Saves the instruction
    char instruction[8]; 

    // 3 Direction Assembly
    char varA[1024]; 
    char varB[1024];
    char varC[1024];
    char varD[1024];
    char ch;


    // Looking for the instruction in the Translated File
    while(fscanf(translated_file, " %8s", instruction) == 1 ){


        if ( strcmp(instruction,"Declare") == 0){
            fscanf(translated_file, " %1023s",varA); //We get the first variable (ID) from the DECLARE
            fprintf(data_file,"    %s: .word 512 \n",varA); // Always a Word because there are no Integers


          
        } else if (strcmp(instruction,"Store")==0){
            fprintf(code_file,"\n@Soy un store\n");
            fscanf(translated_file, " %1023s",varA);//We get the first variable from the store
            fscanf(translated_file, " %1023s",varB);//We get the second variable from the store
            if (atoi(varA) != 0) {// If there is a number
            fprintf(code_file, "    LDR R0, =%s\n    LDR R8, =%s\n    STR R0,[R8]\n", varA, varB);
            } else{// Two ID's
                fprintf(code_file, "    LDR R0,=%s\n    LDR R7,[R0]\n    LDR R8, =%s\n    STR R7,[R8]\n", varA, varB);
            }


        
        } else if (strcmp(instruction,"Add")==0){
            fprintf(code_file,"\n@Soy un add\n");
            fscanf(translated_file, " %1023s",varA);//We get the first variable from the Add 3
            fscanf(translated_file, " %1023s",varB);//We get the second variable from the Add a
            fscanf(translated_file, " %1023s",varC);//We get the second variable from the Add temp1
            
             if (atoi(varA) != 0 && atoi(varB)) {//Two Integers
                fprintf(code_file, "    LDR R1, =%s\n    \n    LDR R2, =%s\n    ADD R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else if (atoi(varA) != 0 && !atoi(varB)) {//Integer and no Integer
                fprintf(code_file, "    LDR R1, =%s\n    LDR R7, =%s\n    LDR R2,[R7]\n    ADD R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else if (!(atoi(varA) != 0) && atoi(varB)) {//No Integer and Integer
                fprintf(code_file, "    LDR R7, =%s\n    LDR R1,[R7]\n    LDR R2, =%s\n    ADD R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else {// Zero Integers
                fprintf(code_file, "    LDR R7, =%s\n    LDR R1,[R7]\n    LDR R7, =%s\n    LDR R2,[R7]\n    ADD R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            }

        
        } else if (strcmp(instruction,"Sub")==0){
            fprintf(code_file,"\n@Soy un Sub\n");
            fscanf(translated_file, " %1023s",varA);
            fscanf(translated_file, " %1023s",varB);
            fscanf(translated_file, " %1023s",varC);
            
            if (atoi(varA) != 0 && atoi(varB)) {//Two Integers
                fprintf(code_file, "    LDR R1, =%s\n    \n    LDR R2, =%s\n    SUB R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else if (atoi(varA) != 0 && !atoi(varB)) {//Integer and no Integer
                fprintf(code_file, "    LDR R1, =%s\n    LDR R7, =%s\n    LDR R2,[R7]\n    SUB R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else if (!(atoi(varA) != 0) && atoi(varB)) {//No Integer and Integer
                fprintf(code_file, "    LDR R7, =%s\n    LDR R1,[R7]\n    LDR R2, =%s\n    SUB R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            } else {// Zero Integers
                fprintf(code_file, "    LDR R7, =%s\n    LDR R1,[R7]\n    LDR R7, =%s\n    LDR R2,[R7]\n    SUB R3, R1, R2\n  LDR R8,=%s\n    STR R3,[R8]",varA,varB,varC);
            }    

        
        } 

        else if (strcmp(instruction,"CMPNZ")==0){
            fprintf(code_file,"\n@Soy un CMPNZ\n");
            fscanf(translated_file, " %1023s",varA);// checkif it's Zero
            fscanf(translated_file, " %1023s",varB);// Zero = true
            fscanf(translated_file, " %1023s",varC);// Zero = false
            fscanf(translated_file, " %1023s",varD);// Variable Id


            if (isdigit(*varA)) //It is a variable
            {
                fprintf(code_file, "    LDR R2,=%s\n    CMP R2,#0\n ",varA);
            }
            else{ //If it's a Integer
                fprintf(code_file, "    LDR R1, =%s\n    \n    LDR R2, [R1]\n    CMP R2,#0\n ",varA); 
            }

            if (atoi(varB)!=0) // Variable B is an Integer (Where VAR A = 0)
            {
                fprintf(code_file, "    LDR R1, =%s\n    LDR R2, =%s\n    STREQ R1,[R2]\n" ,varB,varD);
            }
            else{//Load variable B to memory (Where VAR A = 0)
                fprintf(code_file, "    LDR R3, =%s\n    LDR R1,[R3]\n    LDR R2, =%s\n    STREQ R1,[R2]\n" ,varB,varD);   
            }

            if(atoi(varC)!=0){
                fprintf(code_file, "    LDR R1, =%s\n    LDR R2, =%s\n    STRNE R1,[R2]\n" ,varC,varD);
            }
            else{
                fprintf(code_file, "    LDR R3, =%s\n    LDR R1,[R3]\n    LDR R2, =%s\n    STREQ R1,[R2]\n" ,varC,varD);   
            }
        }else if  (strcmp(instruction,"Read")==0){
            fprintf(code_file,"\n@Soy un Read\n");
            fscanf(translated_file, " %1023s",varA);//Get the first letter from the Add 3
            fprintf(code_file, "    mov R7, #3 \n    mov R0, #1 \n    ldr R1, =%s\n    mov R2, #12\n    svc #0",varA);

        
        }else if  (strcmp(instruction,"Write")==0){
            fprintf(code_file,"\n@Soy un Write\n");
            fscanf(translated_file, " %1023s",varA);//Get the first variable from the Store
            fprintf(code_file, "    push {ip, lr}\n    LDR R8,=%s\n    LDR R1,[R8]\n    ldr R0,=string\n    bl printf\n    pop {ip,pc}",varA);

          
        }else if  (strcmp(instruction,"Halt")==0){
            fprintf(code_file,"\n@Soy un Halt\n");
            fprintf (code_file,"    mov r7, #1 \n    mov r0, #0\n    svc #0\n    bx lr"); 
            break;
        }
     
    }

    fseek(data_file,0,SEEK_SET);
    fseek(code_file,0,SEEK_SET);
    fprintf(rasp_file,".data\n.balign 4\n    string: .asciz \"Result: %%d\"\n");
     while( ( ch = getc(data_file) ) != EOF ){

        fputc(ch,rasp_file);  
     }

     fprintf(rasp_file, "\n\n.text\n.global main\n.extern printf\n\nmain:\n");

     while( ( ch = getc(code_file) ) != EOF ){
        fputc(ch,rasp_file);
     }
    printf("Completed.!\n");
    fclose(data_file);
    fclose(code_file);
    
}
