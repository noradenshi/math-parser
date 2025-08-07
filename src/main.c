#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

int eval(struct Lexer *lexer) {
    struct Token token = lexer_next(lexer);
    token_print(token);

    if (token.type != tok_number) {
        perror("Invalid token");
        exit(1);
    }

    int l = token.value;

    token = lexer_next(lexer);
    token_print(token);

    while (token.type != 0) {
        token = lexer_next(lexer);
        token_print(token);

        int r = token.value;

        token = lexer_next(lexer);
        token_print(token);

        switch (token.type) {
        case '+':
            l += r;
        case '*':
            l += r * eval(lexer);
        }
    }

    return l;
}

int main(void) {
    char buffer[BUFFER_SIZE];

    printf("equation: ");

    if (!fgets(buffer, BUFFER_SIZE, stdin))
        return EXIT_FAILURE;

    struct Lexer lexer;
    lexer_init(&lexer, buffer);

    printf("eval: %d", eval(&lexer));

    return EXIT_SUCCESS;
}
