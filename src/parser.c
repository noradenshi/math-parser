#include "parser.h"
#include "lexer.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode *tree_generate(struct Lexer *lexer) {
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
            node = tree_generate(lexer);
            node->token.precedence += token.precedence;

            if (!root) {
                root = node;
                continue;
            }

            iter->right = node;
            iter = NULL;
            continue;
        }

        node = malloc(sizeof(struct TreeNode));
        node->token = token;
        node->left = iter;
        node->right = NULL;

        iter = NULL;

        if (!prev) {
            root = node;
            continue;
        }

        prev->right = node;
    }

    return root;
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

double tree_eval(struct TreeNode *root) {
    switch (root->token.type) {
    case tok_number:
        return (double)root->token.value;
    case '+':
        if (!root->right) {
            break;
        }

        if (root->left)
            return tree_eval(root->left) + tree_eval(root->right);
        return tree_eval(root->right);
    case '-':
        if (!root->right) {
            break;
        }

        if (root->left)
            return tree_eval(root->left) - tree_eval(root->right);
        return -tree_eval(root->right);
    case '*':
        if (!root->right) {
            break;
        }

        return tree_eval(root->left) * tree_eval(root->right);
    case '/':
        if (!root->right) {
            break;
        }

        return tree_eval(root->left) / tree_eval(root->right);
    default:
        errno = EINVAL;
        printf("Invalid token: %c\n", root->token.type);
        return 0;
    }

    errno = EINVAL;
    printf("Operation contains a NULL: ");
    tree_print(root);
    printf("\n");
    return 0;
}
