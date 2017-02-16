#include "parser.h"
#include <string.h>
//#include "translator.c"

token next_token(void){
	return current_token;
}

void match(token t){
	if(nextev_token == t){
		nextev_token = scanner();
		if (nextev_token == -1) nextev_token = scanner();
		current_token = nextev_token;
		
	}
	else{
	 	syntax_error(t);
		
	 }

}

void syntax_error(token t){
	printf("Exception in Function Paser Type: Syntax Error. \nExpecting %s\n",get_token_name(t));
	exit(0);
}


/*________________FUNCIONES PRINCIPALES DEL PARSER______-*/



/*
SYSTEM GOAL

<system goal> ::= <program> SCANEOF

*/

void system_goal(void){
	program();
	match(SCANEOF);
	finish();
}


/*
PROGRAM

<program> ::= BEGIN <statement list> END

*/


void program(void){
	start();
	match(BEGIN);
	statement_list();
	match(END);
}


/*
STATEMENT LIST

<statement list> ::= <statement>{<statement>

*/


void statement_list(void){
	statement();
	while(true){

		switch(next_token()){

			case ID:

			case READ:

			case WRITE:
				statement();
				break;

			default:
				return;
		}
	}
}

void statement(void){
	token tok= next_token();
	expr_rec result; //Analyze semanticly the Token
	expr_rec p_expr;// Function expression Parameter
	switch(tok){
		case ID:
			
			/*<statement> ::= ID := <expresion>;*/
			strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
			match(ID);
			result = process_id();
			
			//printf("Lo que devuelve el process_id nombre = %s valor = %d tipo = %d\n", result.name, result.val, result.kind);
			match(ASSIGNOP);
			expression(&p_expr); // Le mado la direccion de la variable
			assign(result,p_expr);
			match(SEMICOLON);
			break;
		
		case READ:
		/*<statement> ::= READ(<id_list>);*/
			match(READ);match(LPAREN);
			id_list();match(RPAREN);
			match(SEMICOLON);
			break;

		case WRITE:
		/*<statement>::= WRITE(<expr list>);*/
			match(WRITE);match(LPAREN);
			expr_list();match(RPAREN);
			match(SEMICOLON);
			break;

		default:
			syntax_error(tok);
			break;

	}
}


void assign (expr_rec result, expr_rec p_expr){
	//Generate code for assignment
	char* salida;
	salida = calloc(1024, sizeof(char)); 
	strcpy(salida,extractEXPR(p_expr));
	generate ("Store",salida, result.name,""); 
}




void id_list(void){
	/* <id_list> ::= ID{ ,ID}*/
	expr_rec result;
	strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
	match(ID);
	strcpy(result.name, previous_tokenbuffer);
	read_id(result);
	while(next_token() == COMMA){
		strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
		match(COMMA);
		strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
		match(ID);
		strcpy(result.name, previous_tokenbuffer);
		read_id(result);
	}
}


void read_id (expr_rec in_var){ 
 generate ("Read",in_var.name,"Integer","");
}

void write_expr (expr_rec out_expr){
 generate ("Write", out_expr.name, "Integer", ""); 
}


void expr_list(void){
	/*<expr list>::=<expresion>{ ,<expresion>]*/
	expr_rec result;
	expression(&result);
	write_expr(result);
	while(next_token() == COMMA){
		match(COMMA);
		expression(&result);
		write_expr(result);
	}
}


void expression (expr_rec *result){
	expr_rec left_operand, right_operand;
	op_rec op;
    primary (&  left_operand);
   // printf("El valor del LEFT operand es nombre = %s  valor = %d tipo = %d\n", left_operand.name, left_operand.val, left_operand.kind);
	while (next_token() == PLUSOP || next_token() == MINUSOP){
		add_op (& op); 
		//printf("El valor del OPERATOR es %d\n", op.operator);
		primary (& right_operand);
		//printf("El valor del RIHT operand es nombre = %s  valor = %d tipo = %d\n", right_operand.name, right_operand.val, right_operand.kind);
		left_operand = gen_infix(left_operand, op, right_operand);
	}
	*result = left_operand; 
}



void add_op(op_rec* p_operand){
	token tok = next_token();
	/*<addop>::PLUSOP | MINUSOP*/
	if(tok == PLUSOP || tok==MINUSOP){
		previous_currentToken = current_token; // porque el match me cambia el current token
		match(tok);
		*p_operand = process_op();
	}else{
		syntax_error(tok);
	}
}


void primary(expr_rec* p_operand){
	expr_rec result;
	token tok= next_token();
	switch(tok){
		case LPAREN:
			/*<primary>::= {<expresion>}*/
			match(LPAREN);
			expression(&result);
			*p_operand = result;
			match(RPAREN);
			break;
		case ID:
			/*<primary>::= ID*/
			strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
			match(ID);
			*p_operand = process_id();
			break;
		case INTLITERAL:
			/*<primary>::=INTLITERAL*/
			strcpy(previous_tokenbuffer, token_buffer); // porque el match me cambia el token buffer
			match(INTLITERAL);
			*p_operand = process_literal();
			break;
		default:
			syntax_error(tok);
			break;


	}
}


const char * get_token_name(token t){
	switch(t){
		case BEGIN:
			return "BEGIN";
		case END:
			return "END";
		case READ:
			return "READ";
		case WRITE:
			return "WRITE";
		case ID:
			return "ID";
		case INTLITERAL:
			return "INTLITERAL";
		case LPAREN:
			return "LPAREN";
		case RPAREN:
			return "RPAREN";
		case SEMICOLON:
			return "SEMICOLON";
		case COMMA:
			return "COMMA";
		case ASSIGNOP:
			return "ASSIGNOP";
		case PLUSOP:
			return "PLUSOP";
		case MINUSOP:
			return "MINUSOP";
		case SCANEOF:
			return "SCANEOF";
	}
}


// Translation Operations

void open_outputFile(){
	output_file = fopen ("output_file.txt","w+");
}

void close_outputFile(){
	fclose(output_file);
}

void start(void){
	/*Semantic Initialization , none needed*/
	//printf("%s\n","hola k ace" );
}

void finish(void){
	generate("Halt","","","");
}

void ident(){
	// no sabemos que hace
}


expr_rec process_id(void){
	expr_rec t;
	/*Declare id and build a 
	*corresponding semantic record
	*/
	check_id(previous_tokenbuffer);
	t.kind = IDEXPR;
	strcpy(t.name,previous_tokenbuffer);
	return t;
}


expr_rec process_literal(void){
	expr_rec t;
	/*Convert literal to a numeric representation and build semantic record*/
	t.kind = LITERALEXPR;
	(void) sscanf (previous_tokenbuffer,"%d",& t.val);

	return t; 

}

op_rec process_op (void){
/*Produce operator descriptor*/
	op_rec o;
	if (previous_currentToken == PLUSOP) 
		o.operator = PLUS;
	else
		o.operator = MINUS;
	return o; 
}


void check_id(string s){
	if(! lookup(s)){
		enter(s);
		generate("Declare", s, "Integer", "");
	}
}

bool lookup(string s){
	int i;
	for(i = 0; i < symTable_count; i++){
		if(strcmp(symbol_table[i],s) == 0){
			return true;
		}
		i++;
	}
	return false;	
}

void enter(string s){
	strcpy(symbol_table[symTable_count], s);
	symTable_count++;
}

void generate(string op1,string op2,string op3,string op4){
	//printf("VOY A ESCRIBIRRRR\n");
	if((op1 == "Declare") || (op1 == "Store")){
		fprintf(output_file,"%s %s %s %s\n" ,op1,op2,op3,op4);
	}else if(op1 == "Halt"){
		fprintf(output_file,"%s %s %s %s\n" ,op1,op2,op3,op4);
	}
	else{
		fprintf(output_file,"%s %s %s %s\n" ,op1,op2,op3,op4);
	}

}

char* extractOP(op_rec p_operand){
	//printf("VOY A PONER UN OPERATOR\n");
	char* minus = "Sub";
	char* plus = "Add";
	if (p_operand.operator == MINUS){
		return minus;
	}else if (p_operand.operator == PLUS){
		return plus;
	}
}

char* extractEXPR(expr_rec p_expr){
	char* express_buffer;
	express_buffer = calloc(1024, sizeof(char)); 
	if (p_expr.kind == IDEXPR){
		strcpy(express_buffer,p_expr.name); 
		return express_buffer;
	}else if (p_expr.kind == LITERALEXPR){  
		sprintf (express_buffer,"%d",p_expr.val);
		return express_buffer;
	}else {
		strcpy(express_buffer,p_expr.name); 
		return express_buffer; 
	}  

}

expr_rec gen_infix (expr_rec e1, op_rec op, expr_rec e2) { 
	//printf("ENTRANDO A GENIFIX\n");

	expr_rec e_rec, e_rec1; 
	/*An expr_rec with temp variant set.*/ 
	e_rec.kind = LITERALEXPR; 
	char* resultadoEXPR2; 
	resultadoEXPR2 = calloc(1024, sizeof(char));
	char* resultadoEXPR1; 
	resultadoEXPR2 = calloc(1024, sizeof(char));

	int resultado; 
	/* Generate code for infix operation. 
	Get result temp and set up semantic record for result. */ 
	if (e1.kind == LITERALEXPR && e2.kind == LITERALEXPR) { 
		int numero1 = e1.val; 
		int numero2 = e2.val; 
		if (op.operator == PLUS) { 
			resultado = numero1 + numero2; 
		} else {
			resultado = numero1 - numero2; 
		}  
		e_rec.val = resultado; 
		return e_rec; 
	} else { 
		expr_rec e_rec1; 
		e_rec1.kind = TEMPEXPR; 
		resultadoEXPR1 = extractEXPR(e1); 
		resultadoEXPR2 = extractEXPR(e2); 
		strncpy(e_rec1.name, get_temp(), MAXIDLEN);
		generate (extractOP(op),resultadoEXPR1,resultadoEXPR2,e_rec1.name); 
		return e_rec1; 
	} 
}

char * get_temp (void){ 
/*max temporary allocated so far*/
	static char tempname[MAXIDLEN]; 

	max_temp++;
	sprintf(tempname,"Temp%d", max_temp);

	check_id (tempname);
	return tempname;
}