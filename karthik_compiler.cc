extern "C"{
    #include "compiler.h"
}


struct id_listNode* id_list()
{
    struct idlistNode* idList;
    idList = make_id_listNode();
    ttype = getToken();
    struct varNode* var_node = make_varNode()
    if (ttype == ID)
    {
        idList -> var_node-> name = (char*)malloc(tokenLength+1);
        strcpy(idList->id, token);
        idList -> var_node -> value = 0
        
        ttype = getToken();       
 
        if (ttype == COMMA)
        {
            idList -> id_list = id_list();
            return idList;
        }
        else if (ttype = SEMICOLON)
        {
            ungetToken();
            idList -> id_list = NULL;
            return idList; 
        }

    }       
}


struct var_declNode* var_decl()
{
    struct var_declNode* var_decl_node;
    var_decl_node = make_varDeclNode(); 
    ttype = getToken();
    var_decl_node -> id_list = id_list();
    return var_declNode;
}


struct var_decl_listNode* var_decl_list()
{
    struct var_decl_listNode* var_decl_list_node;
    var_decl_list_node = make_varDeclListNode();
    var_decl_list_node -> var_decl = var_decl();
    
    ttype = getToken();
    if (ttype == ID)
    {
        ungetToken();
        var_decl_list_node -> var_decl_list = var_decl_list() 
        return var_decl_list_node;
    }    
    else
    {
        ungetToken();
        return var_decl_list_node;
    } 
}

struct var_decl_sectionNode* var_decl()
{

    struct var_decl_sectionNode* var_sec_node;
    var_sec_node = make_varSecNode();
    var_sec_node -> var_decl_list = var_decl_list();
    return var_decl_sectionNode;
}

struct stmtNode* stmt()
{
    struct stmtNode* stmt;
    stmt = make_stmtNode();
    ttype = getToken();

    if (ttype == ID)
    {
        ungetToken();
        stmt->assign_stmt = assign_stmt();
        stmt->stmtType = ASSIGN;
        if (ttype == SEMICOLON)
        {
            return stmt;
        } 
    }
}


struct stmt_listNode* stmt_list()
{
    struct stmt_listNode* stmt_list;
    stmt_list = make_stmtListNode()
    stmt_list->stmt = stmt();
    if (ttype == ID)
    {
        ungetToken();
        stmt_list -> stmt_list = stmt_list();
        return stmt_list;
    }
    else
    {
        ungetToken();
        return stmt_list;
    }

}


struct bodyNode* body()
{
    struct bodyNode* body_node;

    ttype = getToken();
    if (ttype == LBRACE)
    {
        body_node = make_bodyNode();
        body_node -> stmt_list = stmt_list();
        ttype = getToken();
        if (ttype == RBRACE)
            return body_node;
    }
}

struct programNode* program()
{
    struct programNode* prog;
    prog = make_programNode();
    prog->decl = var_decl();
    prog->body = body();
    return prog;
}



struct statementNode * parse_program_and_generate_intermediate_representation()
{

    struct ProgramNode* parseTree;
    parseTree = program()


}
