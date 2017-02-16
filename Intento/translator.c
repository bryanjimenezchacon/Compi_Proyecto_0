#include <stdio.h>
#include <ctype.h>
#include "translator.h"

void translate(){
    
    
    // Abrir el translated file generado por el parser con el lenguaje intermedio.
    translated_file = fopen("output_file.txt", "r" );
    mips_file = fopen ("mips_file.asm","w+"); // file en el cual se guardaran las instrucciones en MIPS
    //data_file = fopen ("data_file.txt","w+"); 
    //code_file = fopen ("code_file.txt","w+"); 
    data_file = tmpfile();
    code_file = tmpfile();
    char instruction[8]; //Guardara la instruccion que lea del outputfile. Lo maximo que tendra una instruccion son 8 ch
    char varA[1024]; //Argumentos necesarios en la declaracion de ensamblador de 3 direcciones
    char varB[1024];
    char varC[1024];
    char ch;


    //Cuando matchee la primera palabra, vea que le llego
    while(fscanf(translated_file, " %8s", instruction)==1){

        printf("\n\n");
        fwrite( instruction, 1 ,sizeof(instruction), stdout);
        printf("\n");
  
        
        if (strcmp(instruction,"Declare")==0){
            printf("Soy un declare\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable (ID) del declare
            //fprintf(data_file, "-------------------------------------------------------------------------------------------\n");
            fprintf(data_file,"    %s: .word 0 \n",varA); // Sera siempre un word porque solo hay integers

            //break;
        }else if (strcmp(instruction,"Store")==0){
            printf("Soy un store\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del store
            fscanf(translated_file, " %1023s",varB);//Obtengo la segunda variable del store
            //fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            if (atoi(varA) != 0) { 
               fprintf(code_file, "    li $a0, %s\n    sw $a0, %s\n", varA, varB);
            }else{ 
                fprintf(code_file, "    lw $a0, %s\n    sw $a0, %s\n", varA, varB);
            }
            //fprintf(code_file, "    sw  %s, %s \n",varA, varB);
            //break;
        }else if (strcmp(instruction,"Add")==0){
            printf("Soy un add\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del Add 3
            fscanf(translated_file, " %1023s",varB);//Obtengo la segunda variable del Add a
            fscanf(translated_file, " %1023s",varC);//Obtengo la segunda variable del Add temp1
           // fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            if (atoi(varA) != 0 && atoi(varB)) {
                fprintf(code_file, "    li $a0, %s\n    li $a1, %s\n    add $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else if (atoi(varA) != 0 && !atoi(varB)) {
                fprintf(code_file, "    li $a0, %s\n    lw $a1, %s\n    add $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else if (!atoi(varA) != 0 && atoi(varB)) {
                fprintf(code_file, "    lw $a0, %s\n    li $a1, %s\n    add $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else {
                fprintf(code_file, "    lw $a0, %s\n    lw $a1, %s\n    add $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            }
            
        }else if (strcmp(instruction,"Sub")==0){
            printf("Soy un sub\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del Add 3
            fscanf(translated_file, " %1023s",varB);//Obtengo la segunda variable del Add a
            fscanf(translated_file, " %1023s",varC);//Obtengo la segunda variable del Add temp1
           // fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            if (atoi(varA) != 0 && atoi(varB)) {
                fprintf(code_file, "    li $a0, %s\n    li $a1, %s\n    sub $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else if (atoi(varA) != 0 && !atoi(varB)) {
                fprintf(code_file, "    li $a0, %s\n    lw $a1, %s\n    sub $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else if (!atoi(varA) != 0 && atoi(varB)) {
                fprintf(code_file, "    lw $a0, %s\n    li $a1, %s\n    sub $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            } else {
                fprintf(code_file, "    lw $a0, %s\n    lw $a1, %s\n    sub $v0, $a0, $a1\n    sw $v0, %s\n",varA,varB,varC);
            }

        }else if  (strcmp(instruction,"Read")==0){
            printf("Soy un read\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del Add 3
           // fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            fprintf(code_file, "    li $v0, 5\n    syscall\n    sw $v0, %s\n",varA);
        }else if  (strcmp(instruction,"Write")==0){
            printf("Soy un write\n");
            fscanf(translated_file, " %1023s",varA);//Obtengo la primera variable del store
           // fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            fprintf (code_file,"    li $v0, 1\n    lw $a0, %s \n    syscall \n",varA);   
          
        }else if  (strcmp(instruction,"Halt")==0){
            printf("Soy un halt\n");
            //fprintf(code_file, "-------------------------------------------------------------------------------------------\n");
            fprintf (code_file,"    la $v0, 10\n    syscall\n" ); 
            break;
        }
     
    }

    fseek(data_file,0,SEEK_SET);
    fseek(code_file,0,SEEK_SET);
    fprintf(mips_file,".data\n");
     while( ( ch = getc(data_file) ) != EOF ){

        fputc(ch,mips_file);  
     }
     fprintf(mips_file, ".text\n    main:\n");
     while( ( ch = getc(code_file) ) != EOF ){
        fputc(ch,mips_file);
     }

    fclose(data_file);
    fclose(code_file);
    
}