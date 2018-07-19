
#ifndef SYMDEF_H 
#define SYMDEF_H 

char *keywords[2] = {"main", "if"};	

char *relationalOperators[4] = {"=", "<", ">", "<>"};

char arithOperators[2] = {'+', '-'};

char delimiters[5] = {'(', ')', ',', '[', ']'};

char word[MAX]; // palavra sendo lida
int wi = 0; // index of word string

char numStr[MAX]; // string numerica sendo lida
int ni = 0; // index of numeric string

#endif
