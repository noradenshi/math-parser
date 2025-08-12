#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int get_precedence(const char c) {
    switch (c) {
    case '+':
        return 1;
    case '-':
        return 1;
    case '*':
        return 2;
    case '/':
        return 2;
    case '(':
        return 10;
    default:
        return -1;
    }
}

void token_print(struct Token token) {
    if (token.type == tok_number) {
        printf("(number, %d)\n", token.value);
        return;
    }

    printf("('%c', %d)\n", token.type, token.value);
}

void lexer_init(struct Lexer *lexer, char *data) {
    lexer->data = data;
    lexer->offset = 0;
}

struct Token lexer_next(struct Lexer *lexer) {

    if (lexer->data[lexer->offset] == ' ') {
        lexer->offset++;
        return lexer_next(lexer);
    }

    char *ptr = &lexer->data[lexer->offset];

    if (isdigit(lexer->data[lexer->offset])) {
        lexer->offset++;
        while (isdigit(lexer->data[lexer->offset])) {
            lexer->offset++;
        }

        char tmp = lexer->data[lexer->offset];
        lexer->data[lexer->offset] = 0;
        int value = atoi(ptr);
        lexer->data[lexer->offset] = tmp;

        return (struct Token){tok_number, value};
    }

    lexer->offset++;

    return (struct Token){*ptr, get_precedence(*ptr)};
}
