#ifndef PARSER_H
#define PARSER_H
#endif

#define MAXIDLEN 33
typedef char string[MAXIDLEN];

/* Contains ID'S in case they have been declared */
char symbol_table[1024][1024];


/*For operators*/
typedef struct operator {
	enum op {PLUS,MINUS} operator;
} op_rec;


/*expression types*/
enum expr{IDEXPR,LITERALEXPR,TEMPEXPR};


/*for <primary> and <expression>*/
typedef struct expression{
	enum expr kind;
	union{
		string name; /*for IDEXPR,TEMPEXPR*/
		int val; /*for LITERALEXPR*/

	};
}expr_rec;


/*--------------------------------------------------------------------------------------------------------------------------------------*/

void statement(void);
void id_list(void);
void expression (expr_rec *result);
void expr_list(void);
void add_op(op_rec* p_operand);
void primary(expr_rec* p_operand);
void program(void);
void system_goal(void);
void statement_list(void);

/*--------------------------*/
token next_token();
void match(token t);
void syntax_error(token t);
token current_token;
token nextev_token;
token previous_currentToken;

const char * get_token_name(token t);

/*---------------SEMANTIC ROUTINES FUNCTIONS------------*/

char *symbols[9];
int max_temp = 0;
int cond_counter = 0;
string EC1, EC2, EC3;
int cond_exists = 0;
char target[];

// SEMANTIC ROUTINES
void start();
void finish();
void ident();
void assign(expr_rec target,expr_rec source);
op_rec process_op();
expr_rec gen_infix(expr_rec e1,op_rec op,expr_rec e2);
void read_id(expr_rec in_var);
expr_rec process_id();
expr_rec process_literal(void);
op_rec  process_op(void);
void write_expr(expr_rec out_expr);


// SYMBOL_TABLE_FUNCTIONS
bool lookup(string s); /* Checks if an ID is contained */
void check_id(string s); /* Checks if there is not an ID in the Symbol_Table and adds it*/
void enter(string s); /*Saves ID's into Symbol_Table*/


// FUNCTIONS OVER DATA
void generate();
char* extractOP(op_rec p_operand);
char* extractEXPR(expr_rec p_expr);
expr_rec gen_infix (expr_rec e1, op_rec op, expr_rec e2);
char * get_temp (void);


// OUTPUT FUNCTIONS
void open_outputFile();
void close_outputFile();


// VARIABLES
FILE *output_file;
int symTable_count = 0;
char previous_tokenbuffer[5];