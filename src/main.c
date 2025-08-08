#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64

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
    default:
        return -1;
    }
}

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

int eval(struct Lexer *lexer) {
    struct TreeNode *root = malloc(sizeof(struct TreeNode));
    root->token = lexer_next(lexer);
    root->left = NULL;
    root->right = NULL;

    // token_print(root->token);

    struct Token token;
    struct TreeNode *node = NULL;
    struct TreeNode *iter = NULL;
    struct TreeNode *prev = NULL;

    while ((token = lexer_next(lexer)).type != 0 && token.type != '\n') {
        // token_print(token);

        node = malloc(sizeof(struct TreeNode));
        node->token = token;
        node->left = NULL;
        node->right = NULL;

        if (root->token.type == tok_number) {
            node->left = root;
            root = node;
            continue;
        }

        if (token.type == tok_number) {
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
        while (iter->right && get_precedence(iter->token.type) <
                                  get_precedence(node->token.type)) {
            prev = iter;
            iter = iter->right;
        }

        node->left = iter;
        if (prev) {
            prev->right = node;
        } else {
            root = node;
        }
        iter = NULL;
    }

    int result = tree_eval(root);
    tree_print(root);
    printf("\n");
    tree_free(root);
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
