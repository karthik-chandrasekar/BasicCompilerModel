#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdarg.h>
#include <ctype.h>

//-------------- compiler .h ------------------------

#ifndef _COMPILER_H_
#define _COMPILER_H_

/*
 * compiler.h
 * 
 */
 
#define TRUE 1
#define FALSE 0

#define NOOPSTMT	100
#define PRINTSTMT	101
#define ASSIGNSTMT	102
#define IFSTMT		103		// This is used for all control statements (if, while, repeat)
#define GOTOSTMT	104

#define KEYWORDS	7
#define RESERVED	27
#define VAR			1
#define IF			2
#define WHILE		3 
#define REPEAT		4
#define UNTIL 		5
#define PRINT 		6
#define ARRAY 		7
#define PLUS 		8
#define MINUS 		9
#define DIV 		10
#define MULT 		11
#define EQUAL 		12
#define COLON 		13
#define COMMA 		14
#define SEMICOLON 	15
#define LBRAC 		16
#define RBRAC 		17
#define LPAREN 		18
#define RPAREN 		19
#define LBRACE 		20
#define RBRACE 		21
#define NOTEQUAL 	22
#define GREATER 	23
#define LESS 		24
#define ID 			25
#define NUM 		26
#define ERROR 		27

// This implementation does not allow tokens that are more
// than 100 characters long
#define MAX_TOKEN_LENGTH 100

// The following global variables are defined in compiler.c:
extern char token[MAX_TOKEN_LENGTH];
extern int  ttype;

//---------------------------------------------------------
// Data structures:


struct programNode{
    struct var_decl_sectionNode* decl;
    struct bodyNode* body;
};

struct var_decl_sectionNode
{
    struct var_decl_listNode* var_decl_list;

};

struct var_decl_listNode
{
    struct var_declNode* var_decl;
    struct var_decl_listNode* var_decl_list;
};

struct var_declNode
{
    struct id_listNode* id_list;
};

struct id_listNode
{
    struct varNode* var_node;
    struct id_listNode* id_list;
};

struct varNode
{
	char* name;
	int   value;
};

struct bodyNode
{
    struct stmt_listNode* stmt_list;
};

struct stmt_listNode
{
    struct stmtNode* stmt;
    struct stmt_listNode* stmt_list;
};


struct stmtNode
{
    int stmtType;
    union
    {
        struct gotoStatement* goto_stmt;
        struct assignmentStatement* assign_stmt;
        struct printStatement* print_stmt;
        struct ifStatement* if_stmt;   
    };
};

struct gotoStatement
{
	struct statementNode * target;
};

struct assignmentStatement
{
	struct varNode * lhs;
	struct varNode * op1;
	struct varNode * op2;
	int op;		// PLUS, MINUS, MULT, DIV --> lhs = op1 op op2;
				// 0                      --> lhs = op1;
    struct assignmentStatement* next;

};

struct printStatement
{
	struct varNode * id;
};

struct ifStatement
{
	struct varNode * op1;
	struct varNode * op2;
	int relop;	// GREATER, LESS, NOTEQUAL
	struct statementNode * true_branch;
	struct statementNode * false_branch;
};

struct statementNode
{
	int stmt_type;		// NOOPSTMT, PRINTSTMT, ASSIGNSTMT, IFSTMT, GOTOSTMT
	struct assignmentStatement * assign_stmt;	// NOT NULL iff stmt_type == ASSIGNSTMT
	struct printStatement      * print_stmt;	// NOT NULL iff stmt_type == PRINTSTMT
	struct ifStatement         * if_stmt;		// NOT NULL iff stmt_type == IFSTMT
	struct gotoStatement       * goto_stmt;		// NOT NULL iff stmt_type == GOTOSTMT
	struct statementNode       * next;			// next statement in the list or NULL 
};

//---------------------------------------------------------
// Functions that are provided:

void print_debug(const char * format, ...);
void ungetToken();
int  getToken();

/*
 * You need to write a function with the signature that follows this comment. 
 * It is supposed to parse the input program and generate an intermediate 
 * representation for it. Write all your code in a separate file and include 
 * this header file.
 *
 * If you write your code in C, compile using the following command:
 *
 *   $ gcc compiler.c yourcode.c
 *
 * In case you use C++, use the following commands:
 *
 *   $ gcc -c compiler.c
 *   $ g++ yourcode.cpp compiler.o
 *
 * And also make sure that you include this header file in the following way in
 * your C++ code:
 *
 *  extern "C" {
 *  	#include "compiler.h"
 *  }
 *
 */
struct statementNode * parse_program_and_generate_intermediate_representation();

#endif /* _COMPILER_H_ */



//------------------------------------------------------



/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* 			make_programNode();
struct var_decl_sectionNode*    make_var_decl_sectionNode();
struct var_declNode* 			make_var_declNode();
struct var_decl_listNode* 		make_var_decl_listNode();
struct id_listNode* 			make_id_listNode();
struct bodyNode* 				make_bodyNode();
struct stmt_listNode* 			make_stmt_listNode();
struct stmtNode* 				make_stmtNode();
struct assignmentStatement* 	make_assign_stmtNode();
struct varNode*                 make_varNode();


//---------------------------------------




/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/
struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}


struct var_decl_sectionNode* make_var_decl_sectionNode()
{
	return (struct var_decl_sectionNode*) malloc(sizeof(struct var_decl_sectionNode));
}

struct var_declNode* make_var_declNode()
{
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}


struct var_decl_listNode* make_var_decl_listNode()
{
	return (struct var_decl_listNode*) malloc(sizeof(struct var_decl_listNode));
}

struct id_listNode* make_id_listNode()
{
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}


struct bodyNode* make_bodyNode()
{
	return (struct bodyNode*) malloc(sizeof(struct bodyNode));
}

struct stmt_listNode* make_stmt_listNode()
{
	return (struct stmt_listNode*) malloc(sizeof(struct stmt_listNode));
}

struct stmtNode* make_stmtNode()
{
	return (struct stmtNode*) malloc(sizeof(struct stmtNode));
}


struct assignmentStatement* make_assign_stmtNode()
{
	return (struct assignmentStatement*) malloc(sizeof(struct assignmentStatement));
}


struct varNode* make_varNode()
{
    return (struct varNode*) malloc(sizeof(struct varNode)); 
}

//--------------------------------------




#define DEBUG 1	 // 1 => Turn ON debugging, 0 => Turn OFF debugging

void print_debug(const char * format, ...)
{
	va_list args;
	if (DEBUG)
	{
		va_start (args, format);
		vfprintf (stdout, format, args);
		va_end (args);
	}
}

//---------------------------------------------------------
// Lexer

char token[MAX_TOKEN_LENGTH];	  // token string
int  ttype;						// token type
int  activeToken = FALSE;
int  tokenLength;
int  line_no = 1;

char *reserved[] = 
	 {	"",
		"VAR",
		"IF",
		"WHILE",
		"REPEAT",
		"UNTIL",
		"print",
		"ARRAY",
		"+",
		"-",
		"/",
		"*",
		"=",
		":",
		",",
		";",
		"[",
		"]",
		"(",
		")",
		"{",
		"}",
		"<>",
		">",
		"<",
		"ID",
		"NUM",
		"ERROR"
	 };

void skipSpace()
{
	char c;

	c = getchar(); 
	line_no += (c == '\n');
	while (!feof(stdin) && isspace(c))
	{
		c = getchar(); 
		line_no += (c == '\n');
	}

	// return character to input buffer if eof is not reached
	if (!feof(stdin)) 
		ungetc(c, stdin);
}

int isKeyword(char *s)
{
	int i;

	for (i = 1; i <= KEYWORDS; i++)
		if (strcmp(reserved[i],s) == 0)
			return i;
	return FALSE;
}

int scan_number()
{
	char c;

	c = getchar();
	if (isdigit(c))
	{
		if (c == '0')
		{
			token[tokenLength] = c;
			tokenLength++;
		}
		else
		{
			while (isdigit(c))
			{
				token[tokenLength] = c;
				tokenLength++;;
				c = getchar();
			}
			ungetc(c, stdin);
		}
		token[tokenLength] = '\0';
		return NUM;
	}
	else
		return ERROR;
}


int scan_id_or_keyword()
{
	int ttype;
	char c;

	c = getchar();
	if (isalpha(c))
	{
		while (isalnum(c))
		{
			token[tokenLength] = c;
			tokenLength++;
			c = getchar();
		}
		if (!feof(stdin)) 
			ungetc(c, stdin); 
			 
		token[tokenLength] = '\0';
		ttype = isKeyword(token);
		if (ttype == 0) 
			ttype = ID;
		return ttype;
	} else
		return ERROR;
}

void ungetToken()
{
	activeToken = TRUE;
}

int getToken()
{
	char c;

	if (activeToken)
	{
		activeToken = FALSE;
		return ttype;
	}

	skipSpace();
	tokenLength = 0;
	c = getchar();

	switch (c)
	{
		case '+': ttype = PLUS; break;
		case '-': ttype = MINUS; break;
		case '/': ttype = DIV; break;
		case '*': ttype = MULT; break;
		case '=': ttype = EQUAL; break;
		case ':': ttype = COLON; break;
		case ',': ttype = COMMA; break;
		case ';': ttype = SEMICOLON; break;
		case '[': ttype = LBRAC; break;
		case ']': ttype = RBRAC; break;
		case '(': ttype = LPAREN; break;
		case ')': ttype = RPAREN; break;
		case '{': ttype = LBRACE; break;
		case '}': ttype = RBRACE; break;
		case '<':
			c = getchar();
			if (c == '>')
				ttype = NOTEQUAL;
			else
			{
				ungetc(c, stdin);
				ttype = LESS;
			}
			break;

		case '>':
			ttype = GREATER;
			break;

		default :
			if (isdigit(c))
			{
				ungetc(c, stdin);
				ttype = scan_number();
			}
			else if (isalpha(c)) 
			{
				// token is either a keyword or ID
				ungetc(c, stdin);
				ttype = scan_id_or_keyword();
			}
			else if (c == EOF)
			  ttype = EOF;
			else
			  ttype = ERROR;
			break;
	}	// End Switch
	return ttype;
}

//---------------------------------------------------------
// Execute
void execute_program(struct statementNode * program)
{
	struct statementNode * pc = program;
	int op1, op2, result;

	while (pc != NULL)
	{
		switch (pc->stmt_type)
		{
			case NOOPSTMT:
				pc = pc->next;
				break;

			case PRINTSTMT:
				if (pc->print_stmt == NULL)
				{
					print_debug("Error: pc points to a print statement but pc->print_stmt is null.\n");
					exit(1);
				}
				if (pc->print_stmt->id == NULL)
				{
					print_debug("Error: print_stmt->id is null.\n");
					exit(1);
				}
				printf("%d\n", pc->print_stmt->id->value);
				pc = pc->next;
				break;

			case ASSIGNSTMT:
				if (pc->assign_stmt == NULL)
				{
					print_debug("Error: pc points to an assignment statement but pc->assign_stmt is null.\n");
					exit(1);
				}
				if (pc->assign_stmt->op1 == NULL)
				{
					print_debug("Error: assign_stmt->op1 is null.\n");
					exit(1);
				}
				if (pc->assign_stmt->op == PLUS || pc->assign_stmt->op == MINUS
					|| pc->assign_stmt->op == MULT || pc->assign_stmt->op == DIV)
				{
					if (pc->assign_stmt->op2 == NULL)
					{
						print_debug("Error: right-hand-side of assignment is an expression but assign_stmt->op2 is null.\n");
						exit(1);
					}
				}
				if (pc->assign_stmt->lhs == NULL)
				{
					print_debug("Error: assign_stmt->lhs is null.\n");
					exit(1);
				}
				switch (pc->assign_stmt->op)
				{
					case PLUS:
						op1 = pc->assign_stmt->op1->value;
						op2 = pc->assign_stmt->op2->value;
						result = op1 + op2;
						break;
					case MINUS:
						op1 = pc->assign_stmt->op1->value;
						op2 = pc->assign_stmt->op2->value;
						result = op1 - op2;
						break;
					case MULT:
						op1 = pc->assign_stmt->op1->value;
						op2 = pc->assign_stmt->op2->value;
						result = op1 * op2;
						break;
					case DIV:
						op1 = pc->assign_stmt->op1->value;
						op2 = pc->assign_stmt->op2->value;
						result = op1 / op2;
						break;
					case 0:
						op1 = pc->assign_stmt->op1->value;
						result = op1;
						break;
					default:
						print_debug("Error: invalid value for assign_stmt->op (%d).\n", pc->assign_stmt->op);
						exit(1);
						break;
				}
				pc->assign_stmt->lhs->value = result;
				pc = pc->next;
				break;

			case IFSTMT:
				if (pc->if_stmt == NULL)
				{
					print_debug("Error: pc points to an if statement but pc->if_stmt is null.\n");
					exit(1);
				}
				if (pc->if_stmt->true_branch == NULL)
				{
					print_debug("Error: if_stmt->true_branch is null.\n");
					exit(1);
				}
				if (pc->if_stmt->false_branch == NULL)
				{
					print_debug("Error: if_stmt->false_branch is null.\n");
					exit(1);
				}
				if (pc->if_stmt->op1 == NULL)
				{
					print_debug("Error: if_stmt->op1 is null.\n");
					exit(1);
				}
				if (pc->if_stmt->op2 == NULL)
				{
					print_debug("Error: if_stmt->op2 is null.\n");
					exit(1);
				}
				op1 = pc->if_stmt->op1->value;
				op2 = pc->if_stmt->op2->value;
				switch (pc->if_stmt->relop)
				{
					case GREATER:
						if (op1 > op2)
							pc = pc->if_stmt->true_branch;
						else
							pc = pc->if_stmt->false_branch;
						break;
					case LESS:
						if (op1 < op2)
							pc = pc->if_stmt->true_branch;
						else
							pc = pc->if_stmt->false_branch;
						break;
					case NOTEQUAL:
						if (op1 != op2)
							pc = pc->if_stmt->true_branch;
						else
							pc = pc->if_stmt->false_branch;
						break;
					default:
						print_debug("Error: invalid value for if_stmt->relop (%d).\n", pc->if_stmt->relop);
						exit(1);
						break;
				}
				break;

			case GOTOSTMT:
				if (pc->goto_stmt == NULL)
				{
					print_debug("Error: pc points to a goto statement but pc->goto_stmt is null.\n");
					exit(1);
				}
				if (pc->goto_stmt->target == NULL)
				{
					print_debug("Error: goto_stmt->target is null.\n");
					exit(1);
				}
				pc = pc->goto_stmt->target;
				break;

			default:
				print_debug("Error: invalid value for stmt_type (%d).\n", pc->stmt_type);
				exit(1);
				break;
		}
	}
}




//----------------------------------------

struct id_listNode* parse_id_list()
{
    struct id_listNode* idList;
    idList = make_id_listNode();
    ttype = getToken();
    struct varNode* var_node = make_varNode();
    if (ttype == ID)
    {
        idList->var_node->name = (char*)malloc(tokenLength+1);
        strcpy(idList->var_node->name, token);
        idList->var_node->value = 0;
        
        ttype = getToken();       
 
        if (ttype == COMMA)
        {
            idList->id_list = parse_id_list();
            return idList;
        }
        else if (ttype == SEMICOLON)
        {
            ungetToken();
            idList->id_list = NULL;
            return idList; 
        }

    }       
}


struct var_declNode* parse_var_decl()
{
    struct var_declNode* var_decl_node;
    var_decl_node = make_var_declNode(); 
    ttype = getToken();
    var_decl_node->id_list = parse_id_list();
    return var_decl_node;
}


struct var_decl_listNode* parse_var_decl_list()
{
    struct var_decl_listNode* var_decl_list_node;
    var_decl_list_node = make_var_decl_listNode();
    var_decl_list_node->var_decl = parse_var_decl();
    
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        var_decl_list_node->var_decl_list = parse_var_decl_list(); 
        return var_decl_list_node;
    }    
    else
    {
        ungetToken();
        return var_decl_list_node;
    } 
}

struct var_decl_sectionNode* parse_var_sec()
{
    struct var_decl_sectionNode* var_sec_node;
    var_sec_node = make_var_decl_sectionNode();
    var_sec_node->var_decl_list = parse_var_decl_list();
    return var_sec_node;
}


struct assignmentStatement* parse_assign_stmt()
{
    struct assignmentStatement* assign_stmt;

    assign_stmt = make_assign_stmtNode();

    ttype = getToken();
    struct varNode* lhs = make_varNode();
    assign_stmt->lhs->name = (char*)malloc(tokenLength+1);
    strcpy(assign_stmt->lhs->name, token);
    assign_stmt->lhs->value = 0;

    ttype = getToken();
    if (ttype != EQUAL && ttype == SEMICOLON)
    {
        return assign_stmt;
    }

    ttype = getToken();
    struct varNode* op1 = make_varNode();
    assign_stmt->op1->name = (char*)malloc(tokenLength+1);
    if (ttype == ID)
    {
        strcpy(assign_stmt->op1->name, token);
        assign_stmt->op = 0;
    }
    else
    {
        assign_stmt->op1->value = ttype;     
    
    } 
    ttype = getToken();
    if (ttype == SEMICOLON)
    {
        assign_stmt->op = 0;
        return assign_stmt;
    } 

    else if (ttype != ID)
    {
        assign_stmt->op = ttype;
    }    
    
    ttype = getToken();
    struct varNode* op2 = make_varNode();
    assign_stmt->op2->name = (char*)malloc(tokenLength+1);
    if (ttype == ID)
    {
        strcpy(assign_stmt->op2->name, token);
        assign_stmt->op2->value=0;
    }
    else
    {
        assign_stmt->op2->value = ttype;     
    }
    ttype = getToken();
    if (ttype == SEMICOLON)
    {
        assign_stmt->op = 0;
        return assign_stmt;
    } 
} 

struct stmtNode* parse_stmt()
{
    struct stmtNode* stmt;
    stmt = make_stmtNode();
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        stmt->assign_stmt = parse_assign_stmt();
        stmt->stmtType = ASSIGNSTMT;
        return stmt;
    }
}


struct stmt_listNode* parse_stmt_list()
{
    struct stmt_listNode* stmt_list_node;
    stmt_list_node = make_stmt_listNode();
    stmt_list_node->stmt = parse_stmt();
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        stmt_list_node->stmt_list = parse_stmt_list();
        return stmt_list_node;
    }
    else
    {
        ungetToken();
        return stmt_list_node;
    }
}


struct bodyNode* parse_body()
{
    struct bodyNode* body_node;

    ttype = getToken();
    if (ttype == LBRACE)
    {
        body_node = make_bodyNode();
        body_node->stmt_list = parse_stmt_list();
        ttype = getToken();
        if (ttype == RBRACE)
        {
            return body_node;
        }
    }
}

struct programNode* program()
{
    struct programNode* prog;
    prog = make_programNode();
    prog->decl = parse_var_sec();
    prog->body = parse_body();
    return prog;
}

struct statementNode * parse_program_and_generate_intermediate_representation()
{
    struct programNode* parseTree;
    parseTree = program();

    struct statementNode* execute_format;
    return execute_format; 
}

//---------------------------------------------------------
// Entry point
int main()
{
	struct statementNode * program = parse_program_and_generate_intermediate_representation();
	execute_program(program);
	return 0;
}



