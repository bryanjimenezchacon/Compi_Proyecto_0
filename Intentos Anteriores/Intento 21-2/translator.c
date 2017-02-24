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
    char ch;


    // Looking for the instruction in the Translated File
    while(fscanf(translated_file, " %8s", instruction) == 1 ){


        if ( strcmp(instruction,"Declare") == 0){
            fscanf(translated_file, " %1023s",varA); //Obtengo la primera variable (ID) del declare
            fprintf(data_file,"    %s: .word 512 \n",varA); // Sera siempre un word porque solo hay integers


          
        } else if (strcmp(instruction,"Store")==0){
            fprintf(code_file,"\n@Soy un store\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del store
            fscanf(translated_file, " %1023s",varB);//Obtengo la segunda variable del store
            if (atoi(varA) != 0) { 
               fprintf(code_file, "    LDR R0, #%s\n    LDR R8, =%s\n    STR R0,[R8]\n", varA, varB);
            } else { 
                fprintf(code_file, "    LDR R0,=%s\n    LDR R8, =%s\n    STR R0,[R8]\n", varA, varB);
            }


        
        } else if (strcmp(instruction,"Add")==0){
            fprintf(code_file,"\n@Soy un add\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del Add 3
            fscanf(translated_file, " %1023s",varB);//Obtengo la segunda variable del Add a
            fscanf(translated_file, " %1023s",varC);//Obtengo la segunda variable del Add temp1
            fprintf(code_file, "    LDR R1, =%s\n    LDR R2, =%s\n    add R0, R1, R2\n  LDR R8,=%s\n    STR R0,[R8]",varA,varB,varC);
            

        
        } else if (strcmp(instruction,"Sub")==0){
            fprintf(code_file,"\n@Soy un Sub\n");
            fscanf(translated_file, " %1023s",varA);
            fscanf(translated_file, " %1023s",varB);
            fscanf(translated_file, " %1023s",varC);
            fprintf(code_file, "    LDR R1, =%s\n    LDR R2, =%s\n    sub R0, R1, R2\n  LDR R8,=%s\n    STR R0,[R8]",varA,varB,varC);
            

        
        } else if  (strcmp(instruction,"Read")==0){
            fprintf(code_file,"\n@Soy un Read\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del Add 3
            fprintf(code_file, "    mov R7, #3 \n    mov R0, #1 \n    ldr R1, =%s\n    mov R2, #12\n    svc #0",varA);

        
        } else if  (strcmp(instruction,"Write")==0){
            fprintf(code_file,"@Soy un Write\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del store
            fprintf(code_file, "    push {ip, lr}\n    ldr R3,=string\n    bl printf\n    pop {ip,pc}",varA);
 
          
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