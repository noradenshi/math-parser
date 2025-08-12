#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

struct TreeNode {
    struct Token token;
    struct TreeNode *left;
    struct TreeNode *right;
};

int tree_eval(struct TreeNode *root) {
    switch (root->token.type) {
    case tok_number:
        return root->token.value;
    case '+':
        return tree_eval(root->left) + tree_eval(root->right);
    case '-':
        return tree_eval(root->left) - tree_eval(root->right);
    case '*':
        return tree_eval(root->left) * tree_eval(root->right);
    case '/':
        return tree_eval(root->left) / tree_eval(root->right);
    default:
        return 0;
    }
}

void tree_print(struct TreeNode *root) {
    if (root == NULL)
        return;

    if (root->token.type != tok_number)
        printf("(");

    tree_print(root->left);

    if (root->token.type != tok_number)
        printf(" %c ", root->token.type);
    else
        printf("%d", root->token.value);

    tree_print(root->right);

    if (root->token.type != tok_number)
        printf(")");
}

void tree_free(struct TreeNode *root) {
    if (!root)
        return;

    if (root->left)
        tree_free(root->left);
    if (root->right)
        tree_free(root->right);

    free(root);
}

struct TreeNode *lexer_tree_generate(struct Lexer *lexer) {
    struct Token token;
    struct TreeNode *root = NULL;
    struct TreeNode *node = NULL;
    struct TreeNode *iter = NULL;
    struct TreeNode *prev = NULL;

    while ((token = lexer_next(lexer)).type != 0 && token.type != '\n' &&
           token.type != ')') {
        // token_print(token);

        if (token.type == tok_number) {
            node = malloc(sizeof(struct TreeNode));
            node->token = token;
            node->left = NULL;
            node->right = NULL;

            if (!root) {
                root = node;
                continue;
            }

            iter = root;
            while (iter->right) {
                iter = iter->right;
            }
            iter->right = node;
            iter = NULL;
            continue;
        }

        prev = NULL;
        iter = root;
        while (iter && iter->right &&
               iter->token.precedence < token.precedence) {
            prev = iter;
            iter = iter->right;
        }

        if (token.type == '(') {
            if (!prev)
                prev = root;

            node = lexer_tree_generate(lexer);
            node->token.precedence += token.precedence;

        } else {
            node = malloc(sizeof(struct TreeNode));
            node->token = token;
            node->left = NULL;
            node->right = NULL;

            node->left = iter;
        }

        if (prev) {
            prev->right = node;
        } else {
            root = node;
        }

        iter = NULL;
    }

    return root;
}

int eval(struct Lexer *lexer) {
    struct TreeNode *tree = lexer_tree_generate(lexer);
    int result = tree_eval(tree);

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

    printf("eval: %d\n", eval(&lexer));

    return EXIT_SUCCESS;
}
