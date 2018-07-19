
#ifndef TOKEN_H 
#define TOKEN_H 

#define MAX 1337 // max length of each word string, not including '\0'
#define LIMIT 1337 // max number of word strings in an input file

typedef enum {
	// Identificador: comeca com uma letra e continua com letras ou numeros - nenhum ID eh maior do que MAX
	IDtk, 

	// Palavras Reservadas (main, if) 
	MAINtk, IFtk,

	// Operadores Relacionais (= <  > <>)
	EQUALtk, GREATERtk, LESStk, DIFFtk, 

	// Outros operadores (+  -)
	ADDtk, SUBTRACTtk,

	// Delimitadores ( `(`  `)` `,` `[` `]` )
	LEFTPAtk, RIGHTPAtk, COMMAtk, LEFTBRACKETtk, RIGHTBRACKETtk, 

	// N/A token 
	NAtk, 
	
	// EOF token
	EOFtk

} TokenType;
/* ******************************************************************************* */

struct tokenTag 
{
	// Nome do token
	char str[MAX];
	// Tipo do token
	TokenType tokenType;
	// Linha na qual se encontra o token
	int lineNum;
	// linked-list, used for parse tree
	struct tokenTag *next; 
};
typedef struct tokenTag Token;
/* ******************************************************************************* */

extern int lineNum; // Numero de linhas lidas
extern char tokenTypestr[2 * MAX]; // Temporario utilizado em scanner.c::getTokenTypeStr()

#endif
