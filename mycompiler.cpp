#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <ios>

extern "C"
{
    #include "compiler.h" 
}
using namespace std;

map<string, struct varNode*> var_to_node_map;


struct varNode* make_varNode();
struct statementNode* make_statementNode(); 
struct assignmentStatement* make_assign_stmtNode();
struct printStatement* make_print_stmtNode();
struct ifStatement* make_if_stmtNode();


struct statementNode* parse_body();

struct varNode* make_varNode()
{
    return (struct varNode*) malloc(sizeof(struct varNode)); 
}

struct statementNode* make_statementNode()
{
    return (struct statementNode*) malloc(sizeof(struct statementNode));
}

struct assignmentStatement* make_assign_stmtNode()
{
    return (struct assignmentStatement*) malloc(sizeof(struct assignmentStatement));
}

struct printStatement* make_print_stmtNode()
{
    return (struct printStatement*) malloc(sizeof(struct printStatement));
}

struct ifStatement* make_if_stmtNode()
{
    return (struct ifStatement*) malloc(sizeof(struct ifStatement));
} 

struct gotoStatement* make_goto_stmtNode()
{
    return (struct gotoStatement*) malloc(sizeof(struct gotoStatement));
}

void parse_id_list()
{
    ttype = getToken(); 
    if(ttype != ID)
    {
        ungetToken();
        return;
    }
    struct varNode* var_node;
    var_node = make_varNode();
    var_node->name = (char*)malloc(sizeof(ttype));
    strcpy(var_node->name, token);
    var_node->value = 0;
    
    var_to_node_map[var_node->name] = var_node;

    ttype = getToken();
    if (ttype == COMMA)
    {
        parse_id_list();
    }  
    else if (ttype == SEMICOLON)
    {
        return;
    }    
}


struct assignmentStatement* get_assign_statement()
{
    struct assignmentStatement* assign_stmt;
    assign_stmt = make_assign_stmtNode();

    // LHS
    ttype = getToken();
    struct varNode* lhs;
    lhs = var_to_node_map[token];
    assign_stmt -> lhs = lhs;

    ttype = getToken();
    if (ttype != EQUAL)
    {
        return assign_stmt;
    }     

    //OP1
    ttype = getToken();
    struct varNode* op1;
    if (ttype == ID)
    {
        op1 = var_to_node_map[token];
    }
    else
    {
        op1 = make_varNode();
        op1->value = atoi(token);   
    }
    assign_stmt -> op1 = op1;

    //OP
    ttype = getToken();
    assign_stmt -> op = ttype;

    //OP2
    ttype = getToken();
    struct varNode* op2;
    
    if (ttype == ID)
    {
        op2 = var_to_node_map[token];
    }
    else
    {
        op2 = make_varNode();
        op2->value = atoi(token);    
    }
    assign_stmt -> op2 = op2;

    //SEMICOLON
    ttype = getToken();
    if (ttype == SEMICOLON)
    {
        return assign_stmt;
    }
    return assign_stmt;
}


struct printStatement* get_print_statement()
{
    struct printStatement* print_stmt;
    print_stmt = make_print_stmtNode();
    ttype = getToken();
    if (ttype == ID)
    {
        print_stmt->id = var_to_node_map[token];
    }
    else if  (ttype == NUM)
    {
        print_stmt->id = make_varNode();
        print_stmt->id->value = atoi(token);
    }

    return print_stmt;
}


struct statementNode* get_no_op_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = NOOPSTMT;
    return st;
}


struct ifStatement* get_if_statement()
{
    struct ifStatement* if_stmt;
    struct statementNode* no_op_stmt;
    struct statementNode* temp_stmt;   
 
    if_stmt = make_if_stmtNode();
    ttype = getToken();

    if(ttype == ID)
    {
        if_stmt -> op1 = var_to_node_map[token];
    }
    else
    {
        if_stmt -> op1 = make_varNode();
        if_stmt -> op1 -> value = atoi(token);
    }

    ttype = getToken();
    if_stmt -> relop = ttype;
    
    ttype = getToken();
    
    if(ttype == ID)
    {
        if_stmt -> op2 = var_to_node_map[token];
    }
    else
    {
        if_stmt -> op2 = make_varNode();
        if_stmt -> op2 ->value = atoi(token);
    }

    if_stmt->true_branch = parse_body();

    return if_stmt; 

}

struct statementNode* parse_assign_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = ASSIGNSTMT;
    st -> assign_stmt = get_assign_statement(); 
    return st; 
} 


struct statementNode* parse_print_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = PRINTSTMT;
    st -> print_stmt = get_print_statement();
    return st;

}

struct statementNode* parse_if_stmt()
{
    struct statementNode* st;
    struct statementNode* temp_stmt;    
    struct statementNode* no_op_stmt;

    st = make_statementNode();
    st -> stmt_type = IFSTMT;
    st -> if_stmt = get_if_statement();

    temp_stmt = st->if_stmt->true_branch;

    while(temp_stmt->next != NULL)
    {
        temp_stmt = temp_stmt->next;
    }

    no_op_stmt = make_statementNode();
    no_op_stmt->stmt_type = NOOPSTMT;
    temp_stmt->next = no_op_stmt;

    st->if_stmt->false_branch = no_op_stmt; 

    st -> next = no_op_stmt;
    st = st -> next;
    return st;
}

struct gotoStatement* get_goto_statement()
{
    struct gotoStatement* go_to_stmt;
    return go_to_stmt;
}


struct statementNode* parse_while_stmt()
{
    struct statementNode* st;
    struct statementNode* temp_stmt;

    st = make_statementNode();
    st -> stmt_type = IFSTMT;
    st -> if_stmt = get_if_statement();
    
    temp_stmt = st->if_stmt->true_branch;
    while(temp_stmt->next != NULL)
    {
        temp_stmt = temp_stmt->next;
    }
        
    //GOTO STMT
    struct statementNode* st_go;
    
    st_go = make_statementNode();
    st_go -> stmt_type = GOTOSTMT;
    st_go->goto_stmt = get_goto_statement();

    temp_stmt -> next = st_go;
    st_go->goto_stmt->target = st;
    temp_stmt = temp_stmt -> next;

    //NOOP STMT
    struct statementNode* noop_stmt;
    noop_stmt = get_no_op_stmt();

    st->if_stmt->false_branch = noop_stmt;
   
    st->next = noop_stmt;
    st=st->next;
 
    return st;
}

struct statementNode* parse_stmt()
{
    struct statementNode* st;
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        st = parse_assign_stmt();
        return st;
    }

    else if (ttype == PRINT)
    {
        ungetToken();
        st = parse_print_stmt();
        return st;
    }

    else if (ttype == IF)
    {
        st = parse_if_stmt();
        return st;
    }

    else if (ttype == WHILE)
    {
        ungetToken();
        st = parse_while_stmt();
        return st;
    }
    return st;
}


struct statementNode* parse_stmt_list()
{
    struct statementNode* st;
    struct statementNode* stl;
    st = parse_stmt();
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        stl = parse_stmt_list();
        st->next = stl;
        return st;
    }    
 
    else
    {
        ungetToken();
        return st;
    }
}

struct statementNode* parse_body()
{
    struct statementNode* stmt_node;
    ttype = getToken();
    if (ttype == LBRACE)
    {
        stmt_node = parse_stmt_list();
        ttype = getToken();
        if (ttype == RBRACE)
        {
            return stmt_node;
        }
        return stmt_node;
    }
    return stmt_node;
}


struct statementNode* parse_program_and_generate_intermediate_representation()
{
   struct statementNode* stmt_node;
   parse_id_list();
   stmt_node = parse_body();
   return stmt_node;                
}
