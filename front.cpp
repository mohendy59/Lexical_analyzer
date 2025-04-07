#include <iostream>
#include <cctype>
#include <cstring>
#include <sstream>  // <-- مهم عشان نستخدم istringstream بدل ملف
using namespace std;

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1

/* Global declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen = 0;
int token;
int nextToken;
istream* in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lookup(char ch);
int lex();

/* Add character to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = '\0';
    } else {
        cout << "Error - lexeme is too long\n";
    }
}

/* Get the next character and determine its class */
void getChar() {
    if (in_fp->get(nextChar)) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    } else {
        charClass = END_OF_FILE;
    }
}

/* Skip whitespace characters */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/* Lookup operators and parentheses and return token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            return LEFT_PAREN;
        case ')':
            addChar();
            return RIGHT_PAREN;
        case '+':
            addChar();
            return ADD_OP;
        case '-':
            addChar();
            return SUB_OP;
        case '*':
            addChar();
            return MULT_OP;
        case '/':
            addChar();
            return DIV_OP;
        case '=':
            addChar();
            return ASSIGN_OP;
        default:
            addChar();
            return END_OF_FILE;
    }
}

/* Lexical analyzer */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;

        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        case UNKNOWN:
            nextToken = lookup(nextChar);
            getChar();
            break;

        case END_OF_FILE:
            nextToken = END_OF_FILE;
            strcpy(lexeme, "EOF");
            break;
    }

    cout << "Next token is: " << nextToken << ", Next lexeme is " << lexeme << endl;
    return nextToken;
}

/* Main function */
int main() {
    
    istringstream testInput("a = b + 10 * (c - 2)");
    in_fp = &testInput;

    getChar();
    while (nextToken != END_OF_FILE) {
        lex();
    }

    return 0;
}
