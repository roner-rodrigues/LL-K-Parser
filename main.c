
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "token.h"
#include "scanner.h"
#include "parser.h"

/* *********************************************************************************************** */

// GLOBAL SEG

int lineNum = 1; // extern var from token.h

int numToken; // Numero de tokens lidos
Token *tokens; // Array contendo todos os tokens[numToken]

/* *********************************************************************************************** */

int main(int argc, char *argv[])
{
	int op;
	FILE *filePtr;

	// Limpa a tela
	printf("\033[H\033[J");

	/*---------Begin process cmd-line args and Redirection--------*/

	switch (argc) 
	{
	 case 2: // One parameter: use .lan file supplied	
	  
		if ( (filePtr = fopen(strcat(argv[1], ".lan"), "r")) == NULL ) 
		{
		 printf("Cannot open input file %s.\n", argv[1]);
		 printf("Syntax: LL_K_Parser [file] (.lan is implicit)\n");
		 exit(1);
		}
	 break;

	 default:
		printf("Syntax: LL_K_Parser [file] (.lan is implicit) \n");
		exit(0);
	}
	/*---------End process cmd-line args and Redirection----------*/
	 
	/*--------- Verifica se o arquivo esta vazio -------------*/
	fseek(filePtr, 0, SEEK_END);
	
	if (ftell(filePtr) == 0) 
	{
		printf("File is empty.\n");
		exit(1);
	} else 
	{
		rewind(filePtr);
	}

	/*--------- Verificacao por caracters invalidos -------------*/
	char c;
	int i;
	int numLine = 1;
	int isValid = 1; // assum input char as valid
	int charCount = 0;
	char tempStr[MAX]; 

	while ((c = fgetc(filePtr)) != EOF) {
		if (c == '\n')
			numLine++;

		// Exclude comment line starting with '//'
		if (c == '/') {
			if (fgetc(filePtr) == '/') {
				while ((c = fgetc(filePtr)) != '\n') {
				}
				numLine++;
			}			
		}
		if (isalnum(c)) 
		{
			tempStr[charCount] = c; // array out-of-bound checking not happen here
			charCount++;
			if (charCount > MAX) {
				printf("Word '%s' on line number %d is too long. \n", tempStr, numLine);
				exit(1);	
			}
		} else if (isspace(c) || isAcceptableChar(c)) { 
			//printf("%c is space is extra acceptable character \n", c);
			charCount = 0;
		} else {
			printf("Invalid character '%c' at line %d. \n", c, numLine);
			isValid = 0; // false
		}

	}
	if (isValid == 0) 
	{ 
	 printf("\n Something wrong with the input file - bad characters found. \n");
	 
	 exit(1);
	}

	/*--------- Fim da verificacao por caracters invalidos -------------*/
	
	// Se chegou ate aqui entao nao ha caracters invalidos - analise lexica eh possivel
	printf(" Please, select an option: \n");

	printf("\n (1) \t Lexical Analyser ");
	printf("\n (2) \t Syntactic Analyser ");
	printf("\n (3) \t Show source file \t : ");
	scanf("%d",&op);

	switch(op)
	{
	/*--------- Lexer -------------*/
	 case 1:
	 
	  displaySourceFile(filePtr);

	  do 
	  {
	   numToken++;
	   tokens = (Token *)realloc(tokens, numToken * sizeof(Token));
	   tokens[numToken - 1] = scanner(filePtr);
	   printToken(tokens[numToken - 1]);
	  } 
	  while (!scanner_disabled); 	

	 break;

	/*--------- Parser -------------*/
	 case 2:

	  displaySourceFile(filePtr);

	  parser_LL_K(filePtr); 	

	 break;

	 case 3:

	  displaySourceFile(filePtr);

	 break;
	}

	fclose(filePtr);
	
	return 0;
}

