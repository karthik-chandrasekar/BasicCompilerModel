#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <ios>
#include <fsteam>

extern "C"
{
    #include "compiler.h" 
}
using namespace std;

map<string, varNode> var_to_node_map;

void parse_id_list()
{
    ttype = getToken(); 
    if(ttype != ID)
    {
        ungetToken();
        return;
    }
    struct varNode* var_node;
    var_node = make_var_node();
    var_node->name = (char*)malloc(tokenLength+1);
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

    ttype = getToken();
    struct varNode* lhs;
    lhs  = var_to_node_map[token];
    lhs->name = (char*)malloc(tokenLength+1);
    strcpy(lhs->name, token);
    lhs->value = 0;
    assign_stmt ->lhs =lhs;

    ttype = getToken();
    if (ttype != EQUAL && ttype == SEMICOLON)
    {
        return assign_stmt;
        cout << "end of parse assign stmt"<<"\n";    
    }

    ttype = getToken();
    struct varNode* op1 = var_to_node_map[token];
    op1->name = (char*)malloc(tokenLength+1);
    if (ttype == ID)
    {
        strcpy(op1->name, token);
        assign_stmt->op = 0;
        op1->value = 0;
    }
    else
    {
        op1->value = ttype;     
    }
    assign_stmt -> op1 = op1; 
    ttype = getToken();
    cout << "assignment Statement   "<<ttype<<"\n";
    if (ttype == SEMICOLON)
    {
        assign_stmt->op = 0;
        return assign_stmt;
        cout << "end of parse assign stmt"<<"\n";    
    } 

    else if (ttype != ID)
    {
        assign_stmt->op = ttype;
    }    
    
    ttype = getToken();
    cout << "assignment Statement   "<<ttype<<"\n";
    struct varNode* op2 = var_to_node_map[token];
    op2->name = (char*)malloc(tokenLength+1);
    if (ttype == ID)
    {
        strcpy(op2->name, token);
        op2->value=0;
    }
    else
    {
        op2->value = ttype;     
    }
    assign_stmt -> op2 = op2;
    ttype = getToken();
    cout << "assignment Statement   "<<ttype<<"\n";
    if (ttype == SEMICOLON)
    {
        return assign_stmt;
        cout << "end of parse assign stmt"<<"\n";    
    } 
    cout << "End of parse assign statement   "<<ttype<<"\n";
} 
}

struct printStatement* get_print_statement()
{
    struct printStatement* print_stmt;
    ttype = getToken();
    print_stmt -> id = var_to_node_map[token];
    return print_stmt;
}


struct ifStatement* get_if_statement()
{
    struct ifStatement* if_stmt;
    

}

struct statementNode* parse_assign_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmtType = ASSIGNSTMT;
    st -> assign_stmt = get_assign_statement(); 
    return st; 
} 


struct statementNode* parse_print_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmtType = PRINTSTMT;
    st -> print_stmt = get_print_statement();
    return st;

}

struct statementNode* parse_if_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmtType = IFSTMT;
    st -> print_stmt = get_if_statement();
    return st;
}

struct statementNode* parse_while_stmt()
{
    struct statementNode* st;
    st = make_statementNode();
    st -> stmtType = WHILESTMT;
    st -> print_stmt = get_while_statement();
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
        ungetToken();
        st = parse_if_stmt();
        return st;
    }

    else if (ttype = WHILE)
    {
        ungetToken();
        st = parse_while_stmt();
        return st;
    }
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
    }
}


struct statementNode* parse_program_and_generate_intermediate_representation();
{
   struct statementNode* stmt_node;
   parse_id_list();
   stmt_node = parse_body();
   return stmt_node;                
}
