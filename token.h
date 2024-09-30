#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
typedef enum {
    LE = 256, GE, EQ, NE, LT, GT, CONST_INT,
    CONST_ID, VOID, INT, WHILE,
    IF, ELSE, RETURN, BREAK, CONTINUE
} TokenType;

static void print_token(int token) {
    static char* token_strs[] = {
        "LT", "GE", "EQ", "NE", "LT", "GT", "CONST_INT",
        "CONST_ID", "VOID", "INT", "WHILE",
        "IF", "ELSE", "RETURN", "BREAK", "CONTINUE"
    };

    if (token < 256) {
        printf("%-20c", token);
    } else {
        printf("%-20s", token_strs[token-256]);
    }
}

#endif

