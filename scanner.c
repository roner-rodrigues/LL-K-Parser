
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "scanner.h"
#include "symdef.h"

/* *********************************************************************************************** */

char tokenTypestr[2 * MAX]; // extern defined in token.h

/*------------------------------ Comeco da Analise Lexica ------------------------------*/

Token scanner(FILE *filePtr) 
{
	Token token;
	char ch;

	while ((ch = (char *)fgetc(filePtr)) != EOF) 
	{	
		//printf("filePtr @%p\n", filePtr);
		//printf("LIDO = %c\n", ch);

		if (ch == '\n')
			lineNum++;

		// Ignora comentarios comecando com // ate o final da linha
		else if (ch == '/') {
			if (fgetc(filePtr) == '/') 
			{
				while ((ch = fgetc(filePtr)) != '\n') {}
				lineNum++;
			}
			else
				fseek(filePtr, -1, SEEK_CUR);
		}

		// Verifica se o caracter lido eh alfanumerico 
		else if (isalpha(ch)) 
		{
			word[wi++] = ch;
			
			while (isalnum(ch = fgetc(filePtr))) 
			{
				word[wi++] = ch;
			}
			
			word[wi] = '\0';
			wi = 0;
			//filePtr = (char *)filePtr - 1;
			fseek(filePtr, -1, SEEK_CUR);

			strcpy(token.str, word);

			if (isKeyword(word)) {

				token.tokenType = getTokenTypeOfKeyword(word);
			}
			else {	
				token.tokenType = IDtk;
			}
			token.lineNum = lineNum;
			//printToken(token);

			return token;
		}

		/*
		// Verifica se o caracter eh um digito decimal
		else if (isdigit(ch)) 
		{
			numStr[ni++] = ch;

			ch = fgetc(filePtr);

			while (isdigit(ch)) 
			{
				numStr[ni++] = ch;

				ch = fgetc(filePtr);
			}
			numStr[ni] = '\0';
			ni = 0;
			filePtr = (char *)filePtr - 1;

			strcpy(token.str, numStr);
			token.tokenType = NUMBERtk;
			token.lineNum = lineNum;
			//printToken(token);
			
			return token;
		}
		*/

		// Verifica se o caracter lido eh pontuacao - possivel operador / delimitador
		else if (ispunct(ch)) {
			// Verifica se eh delimitador: `(`, `)`, `[`, `]`, `,`
			if (isDelimiter(ch)) {
				token.tokenType = getTokenTypeOfDelimiter(ch);
				token.lineNum = lineNum;

				char str[2];
				str[0] = ch;
				str[1] = '\0';
				strcpy(token.str, str);

				//printToken(token);
				return token;
			}
			
			// Verifica se eh operador aritmetico: `+`, `-`
			else if (isArithOperator(ch)) {
				token.tokenType = getTokenTypeOfOtherOperator(ch);
				token.lineNum = lineNum;

				char str[2];
				str[0] = ch;
				str[1] = '\0';
				strcpy(token.str, str);

				//printToken(token);
				return token;
			}

			// Verifica se eh operador relacional: `>`,`<`,`<>`,`=`			
			else if (isStartRelationalOperator(ch)) 
			{
				if (ch == '=') 
				{	
					token.lineNum = lineNum;
					
					token.tokenType = EQUALtk;
					strcpy(token.str, "=");
					
					//printToken(token);
					return token;
				}

				else if (ch == '>') 
				{	
					token.lineNum = lineNum;
					
					token.tokenType = GREATERtk;
					strcpy(token.str, ">");
					
					//printToken(token);
					return token;
				}

				else if (ch == '<')
				{
					ch = fgetc(filePtr);

					if (ch == '>') 
					{
					 token.lineNum = lineNum;
					 token.tokenType = DIFFtk;
					 strcpy(token.str, "<>");
					 //printToken(token);
 					 return token;
					}

					else
					{
					 //filePtr = (char *)filePtr - 1;
					 fseek(filePtr, -1, SEEK_CUR);
					 token.lineNum = lineNum;
					 token.tokenType = LESStk;
					 strcpy(token.str, "<");
					 //printToken(token);
 					 return token;
					}
				}
			}
		} // end if ispunct
	} // end while

	rewind(filePtr);

	token.tokenType = EOFtk;
	return token;
}
/*------------------------------ FIM DA ANALISE LEXICA ------------------------------*/

/*------------------------------ FUNCOES AUXILIARES PARA ANALISE DE CARACTER ------------------------------*/

int isKeyword(char *str) 
{
	int i;
	int result = 0; // false
	for (i = 0; i < 2; i++) {
		if (!strcmp(keywords[i], str))
			result = 1;
	}
	return result;

}
/* ********************************************************************************** */

int isDelimiter(char c) 
{
	 int i;
	 int result = 0; // false

	 for (i = 0; i < 5; i++) {
		if (delimiters[i] == c) 
		result = 1;
	 }
	 return result;
}
/* **************************************-******************************************** */

int isArithOperator(char c) 
{
	 int i;
	 int result = 0; // false

	 for (i = 0; i < 2; i++) {
		if (arithOperators[i] == c) 
			result = 1;
	 }
	 return result;
 }
/* ********************************************************************************** */

int isStartRelationalOperator(char c) 
{
	int i;
	int result = 0; // assum false
	
	if (c == '<' || c == '>' || c == '=') 
	{
		result = 1;
	}

	return result;	
}
/* ********************************************************************************** */

TokenType getTokenTypeOfKeyword(char *word) {

	if (strcmp(word, "main") == 0) 	return MAINtk;
	if (strcmp(word, "if") == 0) 	return IFtk;
}
/* ********************************************************************************** */

TokenType getTokenTypeOfDelimiter(char ch) {
	if (ch == '(') return LEFTPAtk;
	if (ch == ')') return RIGHTPAtk;
	if (ch == ',') return COMMAtk;
	if (ch == '[') 
	{
	 //printf("\n HAHAH \n");
	 return LEFTBRACKETtk;	
	}

	if (ch == ']') return RIGHTBRACKETtk;
}
/* ********************************************************************************** */

TokenType getTokenTypeOfOtherOperator(char ch) {
	if (ch == '+') return ADDtk;
	if (ch == '-') return SUBTRACTtk;
}
/* ********************************************************************************** */

// Besides English letters, and digits, these are extra acceptable characters
int isAcceptableChar(char c) 
{
	if (c == '(' || c == ')' || c == ',' ||
	 c == '[' || c == ']' || c == '+' || c == '-' || c == '=' || c == '<' || c == '>') 
	{
	 return 1;
	} 
	
	else return 0;
}
/* ********************************************************************************** */

char *getTokenTypeStr(Token token, char *str) 
{	
	switch (token.tokenType) 
	{
		case IDtk: 	strcpy(str, "IDENTIFER"); break;

		case MAINtk: 	strcpy(str, "main [KEYWORD]"); break; 
		case IFtk: 	strcpy(str, "if [KEYWORD]"); break;

		case LEFTPAtk: 	 	strcpy(str, "left-parenthesis [DELIMITER]"); break; 
		case RIGHTPAtk: 	strcpy(str, "right-parenthesis [DELIMITER]"); break; 
		case COMMAtk: 		strcpy(str, "comma [DELIMITER]"); break; 
		case LEFTBRACKETtk:	strcpy(str, "left-bracket [DELIMITER]"); break;
		case RIGHTBRACKETtk:	strcpy(str, "right-bracket [DELIMITER]"); break;

		case ADDtk:		strcpy(str, "add [OTHER OPERATOR]"); break;
		case SUBTRACTtk:	strcpy(str, "subtractk [OTHER OPERATOR]"); break;

		case EQUALtk: 		strcpy(str, "equal [RELATIONAL OPERATOR]"); break;
		case GREATERtk:		strcpy(str, "greater [RELATIONAL OPERATOR]"); break;
		case LESStk:		strcpy(str, "less [RELATIONAL OPERATOR]"); break;
		case DIFFtk:		strcpy(str, "different [RELATIONAL OPERATOR]"); break;

		default: strcpy(str, "UNKNOWN");
	}
	return str;
}
/* *************************************************************************************** */

void printToken(Token token) 
{
	if (token.tokenType == EOFtk) {
		printf("\n\t ***** EOFtk ***** \n\n");

		scanner_disabled = 1;
		return;
	}

	printf("%10s \t line #%d \t %s \n",token.str, token.lineNum, getTokenTypeStr(token, tokenTypestr));
}
/* *************************************************************************************** */

void displaySourceFile(FILE *filePtr) 
{
	rewind(filePtr);
	printf("\n----- Source file starts here -----\n\n");

	char ch;
	while ((ch = fgetc(filePtr)) != EOF) {
		printf("%c", ch);
	}
	rewind(filePtr);

	printf("\n----- /Source file ends here ------\n\n");
}
/* *************************************************************************************** */
