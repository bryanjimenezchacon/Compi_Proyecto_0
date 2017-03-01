#include "parser.h"
#include <string.h>
//#include "translator.c"

token next_token(void){
	return current_token;
}



/*

MATCH

Recognizes the matching tokens all over the document

*/

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

/*

SYNTAX ERROR

Shows a warning about the misuse of the Syntax

*/


void syntax_error(token t){
	printf("Exception in Function Paser Type: Syntax Error. \nExpecting %s\n",get_token_name(t));
	exit(0);
}


/*________________PARSER'S FUNCTIONS______-*/


/*
SYSTEM GOAL

<system goal> ::= <program> SCANEOF

*/

void system_goal(void){
	program();
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


/*
STATEMENT

<statement list> ::= <statement>{<statement>

*/

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

			// process_id nombre = %s value = %d type = %d\n", result.name, result.val, result.kind);
			match(ASSIGNOP);
			expression(&p_expr); // Variable direction
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


/*

EXPRESSION

<expr list>::=<expresion>{ ,<expresion>]

*/

void isTemp(){
	for (int i = sizeof(symbol_table[symTable_count]) - 1 ; i > 0 ; --i) {
		if( !(strstr(symbol_table[i],"Temp")) && !IsEmptyLine(symbol_table[i])){

			fprintf(output_file," %s\n" ,symbol_table[i]);
		}
	}
}


int IsEmptyLine(char *line){
    while (*line){
        if (!isspace(*line++))
            return 0;
    }
    return 1;
}



void expression (expr_rec *result){
	expr_rec left_operand, right_operand;
	op_rec op;
    primary (&  left_operand);

	while (next_token() == PLUSOP || next_token() == MINUSOP){
		add_op (& op); 
		primary (& right_operand);
		left_operand = gen_infix(left_operand, op, right_operand);
	}

	*result = left_operand; 


	if (next_token() == PIPE){
		conditionals(left_operand, result);
	} 
		
	else if (cond_counter == 2 ){
		char* salida3;
		salida3 = calloc(1024, sizeof(char)); 
		strcpy(salida3,extractEXPR(left_operand));
		strcpy(EC3, salida3);
		cond_counter = 0;
		cond_exists = 1;
	}
}


void conditionals(expr_rec operandos, expr_rec result){


	if (cond_counter == 0){
		char* salida;
		salida = calloc(1024, sizeof(char)); 
		strcpy(salida,extractEXPR(operandos));
		strcpy(EC1, salida);
		cond_counter++;
	}

	else if (cond_counter == 1){
		char* salida2;
		salida2 = calloc(1024, sizeof(char)); 
		strcpy(salida2,extractEXPR(operandos));
		strcpy(EC2, salida2);
		cond_counter++;
	}

	/* FIRST NUMBER
	char* salida;
	salida = calloc(1024, sizeof(char)); 
	strcpy(salida,extractEXPR(operandos));
	printf(" %s - ", salida);*/

	token tok = next_token();
	previous_currentToken = current_token;
	match(tok);
	expression(&result);
}


/* 

EXPR LIST

<expr list>::=<expresion>{ ,<expresion>]

*/



void expr_list(void){
	expr_rec result;
	expression(&result);
	write_expr(result);
	while(next_token() == COMMA){
		match(COMMA);
		expression(&result);
		write_expr(result);
	}
}



/* 

	ID_LIST

<id_list> ::= ID{ ,ID}

*/


void id_list(void){
	// Match change the Token Buffer
	expr_rec result;
	strcpy(previous_tokenbuffer, token_buffer); 
	match(ID);
	strcpy(result.name, previous_tokenbuffer);
	read_id(result);
	while(next_token() == COMMA){
		strcpy(previous_tokenbuffer, token_buffer); 
		match(COMMA);
		strcpy(previous_tokenbuffer, token_buffer); 
		match(ID);
		strcpy(result.name, previous_tokenbuffer);
		read_id(result);
	}
}


/* 

ASSIGN

Generate code for Storing

*/


void assign (expr_rec result, expr_rec p_expr){

		if (cond_exists){
				generate ("CMPNZ",EC1, EC2, EC3, "" ,""); 
				isTemp();
		} else {
	
		char* salida;
		salida = calloc(1024, sizeof(char)); 
		strcpy(salida,extractEXPR(p_expr));
		generate ("Store",salida, result.name,""); 
		}
}

void read_id (expr_rec in_var){ 
 generate ("Read",in_var.name,"Integer","");
}

void write_expr (expr_rec out_expr){
 generate ("Write", out_expr.name, "Integer", ""); 
}


/* 

ADD_OP

Generate code for adding or substracting

<addop>::PLUSOP | MINUSOP

*/


void add_op(op_rec* p_operand){
	token tok = next_token();
	if(tok == PLUSOP || tok==MINUSOP){
		previous_currentToken = current_token;
		match(tok);
		*p_operand = process_op();
	}else{
		syntax_error(tok);
	}
}


/* 

PRIMARY

<primary>::= {<expresion>}

*/


void primary(expr_rec* p_operand){
	expr_rec result;
	token tok= next_token();

	switch(tok){


		case LPAREN:
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


/*

GET_TOKEN_NAME

Returns the String form of the Token

*/


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
}

void finish(void){
	/*Semantic finished*/
	generate("Halt","","","");
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

/* Checks if there is not an ID in the Symbol_Table and generate the instruction*/
void check_id(string s){
	if(! lookup(s)){
		enter(s);
		generate("Declare", s, "Integer", "");
	}
}

/* Checks if an ID is contained */
bool lookup(string s){
	int i;
	for(i = 0; i < symTable_count; i++){
		if(strcmp(symbol_table[i],s) == 0){
			return true;
		}
	}
	return false;	
}

/*Saves ID's into Symbol_Table*/
void enter(string s){
	strcpy(symbol_table[symTable_count], s);
	symTable_count++;
}

/*Writes in the output file*/
void generate(string op1,string op2,string op3,string op4){
	if (op1 == "CMPNZ"){
		fprintf(output_file,"%s %s %s %s" ,op1,op2,op3,op4);
	} else{
		fprintf(output_file,"%s %s %s %s\n" ,op1,op2,op3,op4);
	}
}

char* extractOP(op_rec p_operand){
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

	expr_rec e_rec, e_rec1; 
	/*An expr_rec with temp variant set.*/ 
	e_rec.kind = LITERALEXPR; 
	char* resultEXPR2; 
	resultEXPR2 = calloc(1024, sizeof(char));
	char* resultEXPR1; 
	resultEXPR2 = calloc(1024, sizeof(char));

	int result; 
	/* Generate code for infix operation. 
	Get result temp and set up semantic record for result. */ 
	if (e1.kind == LITERALEXPR && e2.kind == LITERALEXPR) { 
		int number1 = e1.val; 
		int number2 = e2.val; 
		if (op.operator == PLUS) { 
			result = number1 + number2; 
		} else {
			result = number1 - number2; 
		}  
		e_rec.val = result; 
		return e_rec; 
	} else { 
		expr_rec e_rec1; 
		e_rec1.kind = TEMPEXPR; 
		resultEXPR1 = extractEXPR(e1); 
		resultEXPR2 = extractEXPR(e2); 
		strncpy(e_rec1.name, get_temp(), MAXIDLEN);
		generate (extractOP(op),resultEXPR1,resultEXPR2,e_rec1.name); 
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