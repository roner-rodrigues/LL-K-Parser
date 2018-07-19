
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "scanner.h"
#include "parser.h"

/* *********************************************************************************************** */

// GLOBAL SEG

Token tk = {"N/A", NAtk, 0}; 
FILE *fP =	NULL; 

/* *********************************************************************************************** */

// Mark the new node by its type
Node *getNode(NodeType nodeType) 
{
	Node *node;
	node = (Node *)malloc(sizeof(Node));
	node->nodeType = nodeType;
	node->tokenPtr = 	NULL;
	node->child1 = 		NULL;
	node->child2 = 		NULL;
	node->child3 = 		NULL;

	return node;
}
/* *********************************************************************************************** */

void parser_LL_K(FILE *filePtr) 
{
	Node *root = NULL;
	lineNum = 1; // reset line number
	//rewind(fP);
	fP = filePtr;

	tk = scanner(fP); // inicializa o primeiro token

	//printf("First Token is = %s\n", getTokenTypeStr(tk, tokenTypestr));

	//getchar();

	// Inicializa a raiz da arvore de parsing
	//<programa> ::= <principal> <lista-de-funcoes>
	root = programa();

	// Verifica o ultimo token retornado
	if (tk.tokenType == EOFtk) 
		printf("Parser completed successfully. \n");
	else 
	{ 
		printf("\n Something has wrong. Please, try again. \n");

		exit(1);
	}
	
	// Parsing concluido com sucesso - Imprime a arvore de parsing

	printf("\n ****************** Parsing Tree ****************** \n");

	printParseTree(root, 0);

	return;
}
/* *********************************************************************************************** */

//<programa> ::= <principal> <lista-de-funcoes>
Node *programa() 
{
	// Inicializa a raiz da arvore de parsing
	Node *node = getNode(programaNode);

	node->child1 = principal();

	node->child2 = lista_func();
}
/* *********************************************************************************************** */

//<principal> ::= main[<params>](<corpo>)
Node *principal()
{
	Node *node = getNode(principalNode); 

		// MAIN ESPERADO 
		if (tk.tokenType == MAINtk) 
		{
			// insere um token nao terminal 
			insertToken(node, tk);
			// avanca para o proximo token
			tk = scanner(fP);
		} 		
		else 
		{
			printf("ERROR: expect MAINtk, but received %s on line #%d \n", tk.str, tk.lineNum);
			exit(1);
		}

		// LEFTBRACKET ESPERADO 
		if (tk.tokenType == LEFTBRACKETtk) 
		{
			insertToken(node, tk);
			tk = scanner(fP);
		} 		
		else 
		{
			printf("ERROR: expect LEFTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
			exit(1);
		}

		// busca o proximo nao-terminal
		node->child1 = params();

		// RIGHTBRACKET ESPERADO 
		if (tk.tokenType == RIGHTBRACKETtk) 
		{
			insertToken(node, tk);
			tk = scanner(fP);
		} 		
		else 
		{
			printf("ERROR: expect RIGHTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
			exit(1);
		}

		// LEFTPAtk ESPERADO 
		if (tk.tokenType == LEFTPAtk) 
		{
			insertToken(node, tk);
			tk = scanner(fP);
		} 		
		else 
		{
			printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
			exit(1);
		}

		// busca o proximo simbolo nao-terminal
		node->child2 = corpo(0);		

		// RIGHTPAtk ESPERADO 
		if (tk.tokenType == RIGHTPAtk) 
		{
			insertToken(node, tk);
			// avanca para o proximo token
			tk = scanner(fP);
		} 		
		else 
		{
			printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
			exit(1);
		}
}
/* *********************************************************************************************** */

//<params> ::= <id> , <params>
//<params> ::= <id>
Node *params() 
{
	Node *node = getNode(paramsNode);

	node->child1 = id(0); 
	tk = scanner(fP);

 	 //<params> ::= <id> , <params>
	 if(tk.tokenType == COMMAtk)
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	  node->child2 = params();
	  return node;
	 }

  	 //<params> ::= <id>
	 else
	 {
	  return node;
	 }
}
/* *********************************************************************************************** */

//<corpo> ::= <id>[]
//<corpo> ::= <id>[<args>]
//<corpo> ::= if[<cond> , <corpo> , <corpo>]
Node *corpo(int LLK) 
{
	Node *node = getNode(corpoNode);

	//<corpo> ::= if[<cond> , <corpo> , <corpo>]
	if (tk.tokenType == IFtk)
	{
	 insertToken(node, tk);
	 tk = scanner(fP);
	 
	 // LEFTBRACKETtk ESPERADO 
	 if (tk.tokenType == LEFTBRACKETtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	 } 		
	 
 	 else 
	 {
	  printf("ERROR: expect LEFTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }

	 node->child1 = cond();

	 // COMMAtk ESPERADO 
	 if (tk.tokenType == COMMAtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	 } 		
	 
 	 else 
	 {
	  printf("ERROR: expect COMMAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }

	 node->child2 = corpo(0);

	 // COMMAtk ESPERADO 
	 if (tk.tokenType == COMMAtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	 } 		
	 
 	 else 
	 {
	  printf("ERROR: expect COMMAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }

	 node->child3 = corpo(0);

	 // RIGHTBRACKETtk ESPERADO 
	 if (tk.tokenType == RIGHTBRACKETtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	  return node;
	 } 		
	 
 	 else 
	 {
	  printf("ERROR: expect RIGHTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }	
	}

	//<corpo> ::= <id>[]
	//<corpo> ::= <id>[<args>]
	else if (LLK == 1 || tk.tokenType == IDtk)
	{	
	 
	 if(!LLK)
	  tk = scanner(fP);	

	 // LEFTBRACKET ESPERADO 
	 if (tk.tokenType == LEFTBRACKETtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	 } 		
	 
 	 else 
	 {
	  printf("ERROR: expect LEFTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }

	 //<corpo> ::= <id>[]
	 if (tk.tokenType == RIGHTBRACKETtk) 
	 {
	  insertToken(node, tk);
	  node->child1 = id(1); 
	  tk = scanner(fP);
	  return node;
	 }

	 //<corpo> ::= <id>[<args>]
 	 else
	 {
	  node->child1 = id(1); 
	  node->child2 = args();

	  // RIGHTBRACKET ESPERADO 
	  if (tk.tokenType == RIGHTBRACKETtk) 
	  {
	   tk = scanner(fP);
	   return node;
	  } 		

 	  else 
	  {
	   printf("ERROR: expect RIGHTBRACKET, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }
	 }
	}
	 
	else
	{
	 printf("ERROR: expect IDtk or IFtk but received %s on line #%d \n", tk.str, tk.lineNum);
	 exit(1);
	}
}
/* *********************************************************************************************** */

//<cond> ::= <exp> <exp> >
//<cond> ::= <exp> <exp> <
//<cond> ::= <exp> <exp> =
//<cond> ::= <exp> <exp> <>
Node *cond() 
{
	Node *node = getNode(condNode);
	node->child1 = exp();
	node->child2 = exp();

	//<cond> ::= <exp> <exp> >
	if (tk.tokenType == GREATERtk)
	{
	 insertToken(node, tk);
	 tk = scanner(fP);
	 return node;
	}

	//<cond> ::= <exp> <exp> <
	else if (tk.tokenType == LESStk)
	{
	 insertToken(node, tk);
	 tk = scanner(fP);
	 return node;
	}

	
	//<cond> ::= <exp> <exp> =
	else if (tk.tokenType == EQUALtk)
	{
	 insertToken(node, tk);
	 tk = scanner(fP);
	 return node;
	}

	//<cond> ::= <exp> <exp> <>
	else if (tk.tokenType == DIFFtk)
	{
	 insertToken(node, tk);
	 tk = scanner(fP);
	 return node;
	}
	
	else
	{
	 printf("ERROR: expect LESStk, GREATERtk, EQUALtk or DIFFtk but received %s on line #%d \n", tk.str, tk.lineNum);
	 exit(1);
	}		
}
/* *********************************************************************************************** */

//<exp> ::= <id>
//<exp> ::= <corpo>
//<exp> ::= <exp> <exp> +
//<exp> ::= <exp> <exp> -
Node *exp() 
{
	Node *node = getNode(expNode);

	//<exp> ::= <corpo>
	if (tk.tokenType == IFtk)
	{
	 node->child1 = corpo(0);
	 return node;
	}
	 
	//<exp> ::= <id>
	//<exp> ::= <corpo>
	//<corpo> ::= <id>[]
	//<corpo> ::= <id>[<args>]
	else if(tk.tokenType == IDtk)
	{
	 tk = scanner(fP);
	 
	 //<corpo> ::= <id>[]
 	 //<corpo> ::= <id>[<args>]
	 if(tk.tokenType == LEFTBRACKETtk)
	 {
          node->child1 = corpo(1);
	  return node;
	 }
	 
	 //<exp> ::= <id>
	 else 
	 {
	  node->child1 = id(1);
	  return node;
	 }
	}

	//<exp> ::= <exp> <exp> +
	//<exp> ::= <exp> <exp> -
	else
	{
	 node->child1 = exp();
	 node->child2 = exp();

	 if(tk.tokenType == ADDtk || tk.tokenType == SUBTRACTtk)
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	  return node;
	 }
	 
	 else
	 {
	  printf("ERROR: expect ADDtk or SUBTRACTtk but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }
	}
}
/* *********************************************************************************************** */

//<id> ::= IDtk (uma letra seguida de outras letras ou numeros)
Node *id(int LLK) 
{
	Node *node = getNode(idNode);
	
	if(LLK == 1 || tk.tokenType == IDtk)
	 {
	  insertToken(node, tk);
	  //tk = scanner(fP);
	  return node;
	 }
	 
	 else
	 {
	  printf("ERROR: expect IDtk but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }
}
/* *********************************************************************************************** */

//<lista-de-funcoes> ::= <funcao>
//<lista-de-funcoes> ::= <funcao> <lista-de-funcoes>
Node *lista_func() 
{
	Node *node = getNode(lista_funcNode);

	//<lista-de-funcoes> ::= <funcao> <lista-de-funcoes>	
	if(tk.tokenType == IDtk)
	{
	 node->child1 = func();

	 //<lista-de-funcoes> ::= <funcao> <lista-de-funcoes>
	 if(tk.tokenType == IDtk)
	 {
	  node->child2 = lista_func();
	 }

	 //<lista-de-funcoes> ::= <funcao>
 	 else
	 {
	  return node;
	 }
        }
}
/* *********************************************************************************************** */

//<funcao> ::= <id>[](<corpo>)
//<funcao> ::= <id>[<params>](<corpo>)
Node *func() 
{
	Node *node = getNode(funcNode);

	if(tk.tokenType == IDtk)
	{
	 tk = scanner(fP);	 
	
	 // LEFTBRACKET ESPERADO 
	 if (tk.tokenType == LEFTBRACKETtk) 
	 {
	  insertToken(node, tk);
	  tk = scanner(fP);
	 } 		

 	 else 
	 {
	  printf("ERROR: expect LEFTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	  exit(1);
	 }

     	 //<funcao> ::= <id>[<params>](<corpo>)
	 if (tk.tokenType != RIGHTBRACKETtk) 
	 {
	  node->child1 = id(0);
	  node->child2 = params();

	  // RIGHTBRACKET ESPERADO 
	  if (tk.tokenType == RIGHTBRACKETtk) 
	  {
	   insertToken(node, tk);
	   tk = scanner(fP);
	  } 		

 	  else 
	  {
	   printf("ERROR: expect RIGHTBRACKETtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }

	  // LEFTPA ESPERADO 
	  if (tk.tokenType == LEFTPAtk) 
	  {
	   insertToken(node, tk);
	   tk = scanner(fP);
	  } 		

 	  else 
	  {
	   printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }
	  	  
	  node->child3 = corpo(0);

	  // RIGHTPA ESPERADO 
	  if (tk.tokenType == RIGHTPAtk) 
	  {
	   tk = scanner(fP);
	   return node;
	  } 		

 	  else 
	  {
	   printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }		
	 }
	
	 //<funcao> ::= <id>[](<corpo>)
	 else
	 {
	  insertToken(node, tk);
	  node->child1 = id(0); 
	  tk = scanner(fP);

	  // LEFTPA ESPERADO 
	  if (tk.tokenType == LEFTPAtk) 
	  {
	   insertToken(node, tk);
	   tk = scanner(fP);
	  } 		

 	  else 
	  {
	   printf("ERROR: expect LEFTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }

	  node->child2 = corpo(0);

	  // RIGHTPA ESPERADO 
	  if (tk.tokenType == RIGHTPAtk) 
	  {
	   insertToken(node, tk);
	   tk = scanner(fP);
	   return node;
	  } 		

 	  else 
	  {
	   printf("ERROR: expect RIGHTPAtk, but received %s on line #%d \n", tk.str, tk.lineNum);
	   exit(1);
	  }
	 } 
  	} 

	else
	{
	 printf("ERROR: expect IDtk but received %s on line #%d \n", tk.str, tk.lineNum);
	 exit(1);
	}
}
/* *********************************************************************************************** */

//<args> ::= <arg>
//<args> ::= <arg> , <args>
Node *args() 
{
	Node *node = getNode(argsNode);

	node->child1 = arg();

	//<args> ::= <arg> , <args>
	if(tk.tokenType == COMMAtk)
	{
	 insertToken(node, tk);
	 node->child2 = args();
	 return node;
	}

	//<args> ::= <arg>
	else
	{
	 return node;
	}
}
/* *********************************************************************************************** */

//<arg> ::= <exp>
//<arg> ::= <corpo>

/*
<exp> ::= <id>
<exp> ::= <exp> <exp> +
<exp> ::= <exp> <exp> -
<exp> ::= <corpo>
<corpo> ::= <id>[]
<corpo> ::= <id>[<args>]
<corpo> ::= if[<cond> , <corpo> , <corpo>]
*/

Node *arg() 
{
	Node *node = getNode(argNode);

	//<corpo> ::= if[<cond> , <corpo> , <corpo>]
	if(tk.tokenType == IFtk)
	{
	 node->child1 = corpo(0);
	 return node;
	}

	//<exp> ::= <id>
	//<corpo> ::= <id>[]
	//<corpo> ::= <id>[<args>]
	else if(tk.tokenType == IDtk)
	{
	 tk = scanner(fP);
	 
	 //<exp> ::= <corpo>
 	 if(tk.tokenType == LEFTBRACKETtk)
	 {
	  insertToken(node, tk);
	  node->child1 = corpo(1);
	  return node;
	 }

 	 //<exp> ::= <id>
	 else
	 {
	  node->child1 = id(1);
	  return node;
	 }
	}

	//<exp> ::= <exp> <exp> +
	//<exp> ::= <exp> <exp> -
	else
	{
	 node->child1 = exp();
	 return node;
 	}
}
/* *********************************************************************************************** */

/* --------------- PRINT PARSER TREE --------------- */

// Hard-coded to map with enum NodeType defined in parser.h
char *nodeTypeStrArr[] = {
	"<programa>", "<principal>", "<lista_func>", "<func>", "<params>", 
	"<corpo>", "<args>", "<arg>", "<exp>", "<cond>", "<id>"
};

// Hard-coded to map with enum TokenType defined in token.h
char *tokenStrArr[] = {

	"IDtk", "MAINtk", "IFtk", "EQUALtk", "GREATERtk", "LESStk", "DIFFtk", 
	"ADDtk", "SUBTRACTtk", "LEFTPAtk","RIGHTPAtk", "COMMAtk", 
	"LEFTBRACKETtk", "RIGHTBRACKETtk", 
	"NAtk", "EOFtk"
};

void printParseTree(Node *root, int level) 
{
	// printf("%*s" "%s ", level * 3, "", nodeTypeStrArr[root->nodeType]);
/*
	if (root->token.tokenType != NAtk && root->token.tokenType != EOFtk) {
		// printf(" Token on line #%d is %s", root->token.lineNum, root->token.str); 
		printf(" [Token %s on line #%d]", root->token.str, root->token.lineNum); 
	}
*/
	Token *tmp = root->tokenPtr;
	
	int isTokenFound = 0; // false
	
	while (tmp != NULL) 
	{
	 isTokenFound = 1;
	 
	 printf("{Token(s) found: ");	
	 
	 int isLastToken = 0; // false

	 //printf("getTokenTypeStr(tk, tokenTypestr)= %d !\n", getTokenTypeStr(tk, tokenTypestr));
		
	 printf("%s (%s, #%d)", tmp->str, tokenStrArr[tmp->tokenType], tmp->lineNum);
		
	 tmp = tmp->next;
			
	 //isLastToken = 0;
	 if (tmp != NULL)
	 {
	  printf(", and ");		
	 } 
	}

	if (isTokenFound) 
	{
	 printf("} \n");
	}

	printParseTree(root->child1, level + 1);
	printParseTree(root->child2, level + 1);
	printParseTree(root->child3, level + 1);

	return;
}
/* *********************************************************************************************** */

/* --------------- END OF PARSER TREE PRINTING --------------- */

Token *getTokenPtr(Token tk) {

	Token *tokenPtr = (Token *) malloc(sizeof(Token));
	//printf("tk.str = %s", tk.str);
	strcpy(tokenPtr->str, tk.str);
	tokenPtr->lineNum = tk.lineNum;
	tokenPtr->tokenType = tk.tokenType;
	tokenPtr->next = NULL;

	return tokenPtr;
}
/* *********************************************************************************************** */

// Insert new token at the end of the linked-list of tokens 
void insertToken(Node *node, Token tk) 
{
	Token *new = getTokenPtr(tk);

	if (node->tokenPtr == NULL) 
	{
	 node->tokenPtr = new;
	}

	else 
	{
		Token *tmp = node->tokenPtr;
		while (tmp->next != NULL) 
		{
		 tmp = tmp->next;
		}
		
		tmp->next = new;
	}
}
/* *********************************************************************************************** */
