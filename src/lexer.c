#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

    if (isdigit(lexer->data[lexer->offset])) {
        char *num = &lexer->data[lexer->offset];
        lexer->offset++;
        while (isdigit(lexer->data[lexer->offset])) {
            lexer->offset++;
        }

        char tmp = lexer->data[lexer->offset];
        lexer->data[lexer->offset] = 0;
        int value = atoi(num);
        lexer->data[lexer->offset] = tmp;

        return (struct Token){tok_number, value};
    }

    return (struct Token){lexer->data[lexer->offset++], 0};
}
