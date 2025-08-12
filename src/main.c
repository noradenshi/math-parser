#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

double eval(struct Lexer *lexer) {
    struct TreeNode *tree = tree_generate(lexer);
    double result = tree_eval(tree);

    tree_print(tree);
    printf("\n");

    tree_free(tree);

    return result;
}

int main(void) {
    char buffer[BUFFER_SIZE];

    printf("equation: ");

    if (!fgets(buffer, BUFFER_SIZE, stdin))
        return EXIT_FAILURE;

    struct Lexer lexer;
    lexer_init(&lexer, buffer);

    double result = eval(&lexer);

    printf("eval: %g\n", result);

    return EXIT_SUCCESS;
}
