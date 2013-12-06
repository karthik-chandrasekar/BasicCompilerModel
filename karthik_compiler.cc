#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "compiler.h"









/*--------------------------------------------------------------------
  CREATING PARSE TREE NODE
---------------------------------------------------------------------*/
struct programNode* make_programNode()
{	
	return (struct programNode*) malloc(sizeof(struct programNode));
}

struct declNode* make_declNode()
{
	return (struct declNode*) malloc(sizeof(struct declNode));
}

struct type_decl_sectionNode* make_type_decl_sectionNode()
{
	return (struct type_decl_sectionNode*) malloc(sizeof(struct type_decl_sectionNode));
}

struct var_decl_sectionNode* make_var_decl_sectionNode()
{
	return (struct var_decl_sectionNode*) malloc(sizeof(struct var_decl_sectionNode));
}

struct var_declNode* make_var_declNode()
{
	return (struct var_declNode*) malloc(sizeof(struct var_declNode));
}

struct type_declNode* make_type_declNode()
{
	return (struct type_declNode*) malloc(sizeof(struct type_declNode));
}

struct type_decl_listNode* make_type_decl_listNode()
{
	return (struct type_decl_listNode*) malloc(sizeof(struct type_decl_listNode));
}

struct var_decl_listNode* make_var_decl_listNode()
{
	return (struct var_decl_listNode*) malloc(sizeof(struct var_decl_listNode));
}

struct id_listNode* make_id_listNode()
{
	return (struct id_listNode*) malloc(sizeof(struct id_listNode));
}

struct type_nameNode* make_type_nameNode()
{
	return (struct type_nameNode*) malloc(sizeof(struct type_nameNode));
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

struct while_stmtNode* make_while_stmtNode()
{
	return (struct while_stmtNode*) malloc(sizeof(struct while_stmtNode));
}

struct assign_stmtNode* make_assign_stmtNode()
{
	return (struct assign_stmtNode*) malloc(sizeof(struct assign_stmtNode));
}

struct exprNode* make_exprNode()
{
	return (struct exprNode*) malloc(sizeof(struct exprNode));
}

struct primaryNode* make_primaryNode()
{
	return (struct primaryNode*) malloc(sizeof(struct primaryNode));
}

struct conditionNode* make_conditionNode()
{
    return (struct conditionNode*) malloc(sizeof(struct conditionNode));
}

//--------------------------------------


/*------------------------------------------------------------------------
  PARSE TREE FUNCTIONS
--------------------------------------------------------------------------*/
struct programNode* 			make_programNode();
struct declNode* 				make_declNode();
struct type_decl_sectionNode* 	make_type_decl_sectionNode();
struct var_decl_sectionNode* 	make_var_decl_sectionNode();
struct type_decl_listNode*		make_type_decl_listNode();
struct type_declNode* 			make_type_declNode();
struct var_decl_listNode* 		make_var_decl_listNode();
struct var_declNode* 			make_var_declNode();
struct id_listNode* 			make_id_listNode();
struct type_nameNode* 			make_type_nameNode();
struct bodyNode* 				make_bodyNode();
struct stmt_listNode* 			make_stmt_listNode();
struct stmtNode* 				make_stmtNode();
struct while_stmtNode* 			make_while_stmtNode();
struct assign_stmtNode* 		make_assign_stmtNode();
struct type_nameNode* 			make_type_nameNode();
struct exprNode* 				make_exprNode();
struct primaryNode* 			make_primaryNode();
struct conditionNode*           make_conditionNode();



//---------------------------------------

struct id_listNode* parse_id_list()
{
    struct idlistNode* idList;
    idList = make_id_listNode();
    ttype = getToken();
    struct varNode* var_node = make_varNode();
    if (ttype == ID)
    {
        idList->var_node->name = (char*)malloc(tokenLength+1);
        strcpy(idList->var_node->name, token);
        idList->var_node->value = 0
        
        ttype = getToken();       
 
        if (ttype == COMMA)
        {
            idList->id_list = parse_id_list();
            return idList;
        }
        else if (ttype = SEMICOLON)
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
    var_decl_node = make_varDeclNode(); 
    ttype = getToken();
    var_decl_node->id_list = parse_id_list();
    return var_declNode;
}


struct var_decl_listNode* parse_var_decl_list()
{
    struct var_decl_listNode* var_decl_list_node;
    var_decl_list_node = make_varDeclListNode();
    var_decl_list_node->var_decl = parse_var_decl();
    
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        var_decl_list_node->var_decl_list = parse_var_decl_list() 
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
    var_sec_node = make_varSecNode();
    var_sec_node->var_decl_list = parse_var_decl_list();
    return var_decl_sectionNode;
}


struct assignmentStatement* parse_assign_stmt()
{
    struct assignmentStatement* assign_stmt;

    assign_stmt = make_assign_stmtNode()

    ttype = getToken();
    struct varNode* lhs = make_varNode();
    assign_stmt->lhs->name = (char*)malloc(tokenLength+1);
    strcpy(assign_stmt->lhs->name, token);
    assign_stmt->lhs->value = 0;

    ttype = getToken()
    if (ttype != EQUAL && ttype == SEMICOLON)
    {
        return assign_stmt;
    }

    ttype = getToken()
    struct varNode* op1 = make_varNode()
    assign_stmt->op1->name = (char*)malloc(tokenLength+1);
    if (ttype == ID)
    {
        strcpy(assign_stmt->op1->name, token);
        assign_stmt->value = 0
    }
    else
    {
        assign_stmt->op1->value = ttype;     
    
    } 
    ttype = getToken()
    if (ttype == SEMICOLON)
    {
        assign_stmt->op = 0;
        return assign_stmt;
    } 

    else if (ttype != ID)
    {
        assign_stmt->op = ttype;
    }    
    
    ttype = getToken()
    struct varNode* op2 = make_varNode()
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
    ttype = getToken()
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
        stmt->stmtType = ASSIGN;
        return stmt;
    }
}


struct stmt_listNode* parse_stmt_list()
{
    struct stmt_listNode* stmt_list_node;
    stmt_list_node = make_stmtListNode()
    stmt_list_node->stmt = parse_stmt();
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        stmt_list_node->stmt_list = stmt_list();
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
    struct ProgramNode* parseTree;
    parseTree = program()
    return parseTree;
}
