#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int get_precedence(const char c) {
    switch (c) {
    case tok_number:
        return 1;
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
        return 0;
    }
}

struct Token token_juxta() {
    return (struct Token){0, get_precedence('*'), '*'};
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

    char *ptr = &lexer->data[lexer->offset];

    if (*ptr == ' ') {
        lexer->offset++;
        return lexer_next(lexer);
    }

    if (isdigit(*ptr) && *(ptr - 1) == ')') {
        *(ptr - 1) = '*';
        return token_juxta();
    }

    if (*ptr == '(' && (isdigit(*(ptr - 1)) || *(ptr - 1) == ')')) {
        *(ptr - 1) = '*';
        return token_juxta();
    }

    if (isdigit(*ptr)) {
        lexer->offset++;
        while (isdigit(lexer->data[lexer->offset])) {
            lexer->offset++;
        }

        char tmp = lexer->data[lexer->offset];
        lexer->data[lexer->offset] = 0;
        int value = atoi(ptr);
        lexer->data[lexer->offset] = tmp;

        return (struct Token){value, get_precedence(tok_number), tok_number};
    }

    lexer->offset++;

    return (struct Token){0, get_precedence(*ptr), *ptr};
}
