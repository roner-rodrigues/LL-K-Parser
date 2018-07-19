#ifndef SCANNER_H 
#define SCANNER_H 

/* PROTOTIPO DAS FUNCOES UTILIZADAS */

int isAcceptableChar(char);
int isDelimiter(char);
int isArithOperator(char);
int isStartRelationalOperator(char);
int isKeyword(char *);

Token scanner(FILE *);

void printToken(Token);

TokenType getTokenTypeOfKeyword(char *);
TokenType getTokenTypeOfDelimiter(char);
TokenType getTokenTypeOfOtherOperator(char);

void displaySourceFile(FILE *);

int scanner_disabled;

#endif


