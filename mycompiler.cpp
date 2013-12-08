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
struct gotoStatement* make_goto_statementNode();

struct statementNode* parse_body();


struct gotoStatement* make_goto_statementNode()
{
    return (struct gotoStatement*) malloc(sizeof(struct gotoStatement));
}

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
    //cout << "Begin of parse id list"<<"\n";
    ttype = getToken(); 
    if(ttype != ID)
    {
        ungetToken();
        return;
        //cout << "End of parse id list - 1"<<"\n";
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
        //cout << "End of parse id list - 2"<<"\n";
        return;
    }
    //cout << "End of parse id list -3 "<<"\n";
}


struct assignmentStatement* get_assign_statement()
{
    //cout << "get assign statement - Begin"<<"\n";
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
        //cout << "get assign statement - end 1"<<"\n";
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
    if (ttype == SEMICOLON)
    {
        //cout << "get assign statement - end 2"<<"\n";
        assign_stmt -> op = 0;
        return assign_stmt;
    }
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
        //cout << "get assign statement - end 3"<<"\n";
        return assign_stmt;
    }
    //cout << "get assign statement - end 4"<<"\n";
    return assign_stmt;
}


struct printStatement* get_print_statement()
{
    //cout << "get print statement - begin"<<"\n";
    struct printStatement* print_stmt;
    print_stmt = make_print_stmtNode();
    ttype = getToken();
    if (ttype != PRINT)
    {
        //cout << "get print statement - end1"<<"\n";
        return print_stmt; 
    }
    ttype = getToken();
    if (ttype == ID)
    {
        print_stmt->id = var_to_node_map[token];
    }
    else 
    {
        print_stmt->id = make_varNode();
        print_stmt->id->value = atoi(token);
    }
    
    ttype = getToken();
    if (ttype == SEMICOLON)
    {
        //cout << "get print statement - end2"<<"\n";
        return print_stmt;
    }
    //cout << "get print statement - end3"<<"\n";
    return print_stmt;
}


struct statementNode* get_no_op_stmt()
{
    //cout << "get no op statement - begin"<<"\n";
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = NOOPSTMT;
    //cout << "get no op statement - end"<<"\n";
    return st;
}


struct ifStatement* get_if_statement()
{
    //cout << "get if  statement - begin"<<"\n";
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

    //cout << "get if op statement - end"<<"\n";
    return if_stmt; 

}

struct statementNode* parse_assign_stmt()
{
    //cout<<"Parse assign stmt - beigin"<<"\n"; 
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = ASSIGNSTMT;
    st -> assign_stmt = get_assign_statement(); 
    //cout<<"Parse assign stmt - end"<<"\n"; 
    return st; 
} 


struct statementNode* parse_print_stmt()
{
    //cout<<"Parse print stmt - beigin"<<"\n"; 
    struct statementNode* st;
    st = make_statementNode();
    st -> stmt_type = PRINTSTMT;
    st -> print_stmt = get_print_statement();
    //cout<<"Parse print stmt - end"<<"\n"; 
    return st;

}

struct statementNode* parse_if_stmt()
{
    //cout<<"Parse if stmt - beigin"<<"\n"; 
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
    //cout<<"Parse if stmt - end"<<"\n"; 
    return st;
}

struct gotoStatement* get_goto_statement()
{
    //cout<<"Parse go to e stmt - beigin"<<"\n"; 
    struct gotoStatement* go_to_stmt;
    go_to_stmt = make_goto_statementNode();
    //cout<<"Parse go to stmt - end"<<"\n"; 
    return go_to_stmt;
}


struct ifStatement* get_repeat_if_statement()
{
    //cout << "REPEAT IF Statement - begins"<<"\n";
    struct ifStatement* if_stmt;
    
    if_stmt = make_if_stmtNode();
    ttype = getToken();
    //cout << "REPEAT IF -  "<<ttype<<"\n";

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
    //cout << "REPEAT IF -  "<<ttype<<"\n";
    if_stmt -> relop = ttype;
    ttype = getToken();
    //cout << "REPEAT IF -  "<<ttype<<"\n";
    
    if(ttype == ID)
    {
        if_stmt -> op2 = var_to_node_map[token];
    }
    else
    {
        if_stmt -> op2 = make_varNode();
        if_stmt -> op2 ->value = atoi(token);
    }

    //cout << "REPEAT IF Statement - Ends"<<"\n";
    return if_stmt;
}

struct statementNode* parse_repeat_stmt()
{
   
    //cout << "Parse repeat statement begins"<<"\n"; 
    struct statementNode* st;
    struct statementNode* body_stmt;
    struct statementNode* temp_stmt;

    body_stmt = parse_body();

    st = make_statementNode();
    st -> stmt_type = IFSTMT;
    ttype = getToken();
    st -> if_stmt = get_repeat_if_statement();
    st -> if_stmt -> true_branch = body_stmt;

    temp_stmt = body_stmt;
   
    while (temp_stmt->next != NULL)
    {
        temp_stmt = temp_stmt->next;
    }

    temp_stmt -> next = st;
 
    struct statementNode* noop_stmt;
   
    noop_stmt = get_no_op_stmt();
    st -> if_stmt->false_branch = noop_stmt;
    st->next = noop_stmt;
    
    //cout << "Parse repeat statement ends"<<"\n";
    return body_stmt; 

}


struct statementNode* parse_while_stmt()
{

    //cout<<"Parse while stmt - beigin"<<"\n"; 
    struct statementNode* st;
    struct statementNode* temp_stmt;

    st = make_statementNode();
    st -> stmt_type = IFSTMT;
    st -> if_stmt = get_if_statement();
    
    temp_stmt = st->if_stmt->true_branch;
    //cout<<"INSIDE WHILE "<<"\n";
    while( temp_stmt !=NULL and temp_stmt->next != NULL)
    {
        temp_stmt = temp_stmt->next;
    }
    //cout<<"INSIDE WHILE GOTO"<<"\n";
        
    //GOTO STMT
    struct statementNode* st_go;
    
    st_go = make_statementNode();
    //cout<<"INSIDE WHILE GOTO"<<"\n";
    st_go -> stmt_type = GOTOSTMT;
    st_go->goto_stmt = get_goto_statement();

    temp_stmt -> next = st_go;
    st_go->goto_stmt->target = st;
    temp_stmt = temp_stmt -> next;

    //cout<<"BEFORE NOOP STMT"<<"\n";
    //NOOP STMT
    struct statementNode* noop_stmt;
    noop_stmt = get_no_op_stmt();

    st->if_stmt->false_branch = noop_stmt;
   
    st->next = noop_stmt;
    //cout<<"Parse while stmt - end"<<"\n"; 
    return st;
}

struct statementNode* parse_stmt()
{
    //cout<<"Parse stmt - begin"<<"\n"; 
    struct statementNode* st;
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        st = parse_assign_stmt();
        //cout<<"Parse stmt - end - 1"<<"\n"; 
        return st;
    }

    else if (ttype == PRINT)
    {
        ungetToken();
        st = parse_print_stmt();
        //cout<<"Parse stmt - end - 2"<<"\n"; 
        return st;
    }

    else if (ttype == IF)
    {
        st = parse_if_stmt();
        //cout<<"Parse stmt - end - 3"<<"\n"; 
        return st;
    }

    else if (ttype == WHILE)
    {
        st = parse_while_stmt();
        //cout<<"Parse stmt - end - 4"<<"\n"; 
        return st;
    }

    else if (ttype == REPEAT)
    {
        st = parse_repeat_stmt();
        //cout<<"Parse stmt - end - 5"<<"\n"; 
        return st; 
    }
    return st;
}


struct statementNode* parse_stmt_list()
{
    //cout<<"Parse stmt list"<<"\n"; 
     
    struct statementNode* st;
    struct statementNode* stl;
    struct statementNode* head;

    st = parse_stmt();
    head = st;
    ttype = getToken();
    if (ttype == ID || ttype == WHILE || ttype == PRINT || ttype == IF || ttype == REPEAT)
    {
        ungetToken();
        stl = parse_stmt_list();
        if (st -> next!=NULL)
        {
            if (st -> next -> stmt_type == NOOPSTMT)
            {
                st = st->next;
            }
        }
        st->next = stl;
        //cout<<"Parse stmt list - end 1"<<"\n"; 
        return head;
    }    
 
    else
    {
        ungetToken();
        //cout<<"Parse stmt list - end 2"<<"\n"; 
        return head;
    }
}

struct statementNode* parse_body()
{
    //cout<<"Parse body"<<"\n"; 
    struct statementNode* stmt_node;
    ttype = getToken();
    if (ttype == LBRACE)
    {
        stmt_node = parse_stmt_list();
        ttype = getToken();
        if (ttype == RBRACE)
        {
            //cout<<"End parse body 1"<<"\n"; 
            return stmt_node;
        }
        //cout<<"End parse body 2"<<"\n"; 
        return stmt_node;
    }
    //cout<<"End parse body 3"<<"  "<<ttype<<"\n"; 
    return stmt_node;
}


struct statementNode* parse_program_and_generate_intermediate_representation()
{
   struct statementNode* stmt_node;
   parse_id_list();
   stmt_node = parse_body();
   return stmt_node;                
}
