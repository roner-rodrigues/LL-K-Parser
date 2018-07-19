
#ifndef PARSER_H 
#define PARSER_H 

/* *********************************************************************************************** */

// non-terminal symbols
typedef enum {
	programaNode, principalNode, lista_funcNode, funcNode, paramsNode, corpoNode,
	argsNode, argNode, expNode, condNode, idNode
} NodeType;

/* --------------------- ARVORE DE PARSING --------------------- */

// UM NODO PARA CADA SIMBOLO NAO TERMINAL
struct nodeTag 
{
	NodeType nodeType;
	// linked-list of tokens of this node 
	Token *tokenPtr; 
	// Maximo de 3 filhos necessarios - um para cada nao-terminal do lado direito do simbolo
	struct nodeTag *child1; 
	struct nodeTag *child2;
	struct nodeTag *child3;
};
typedef struct nodeTag Node;
/* *********************************************************************************************** */

void parser_LL_K(FILE *);

// Cada simbolo nao terminal tem sua funcao de inicializacao 
Node * programa();
Node * principal();
Node * lista_func();
Node * func();
Node * params();
Node * corpo(int);
Node * args();
Node * arg();
Node * exp();
Node * cond();
Node * id(int);

// Inicializa o nodo de acordo com seu tipo
Node * getNode(NodeType);
// Insere um token na arvore de parsing
void insertToken(Node *, Token);
// Imprime a arvore de parsing
void printParseTree(Node *, int);

/* *********************************************************************************************** */

#endif
