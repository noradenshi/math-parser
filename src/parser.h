#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

struct TreeNode {
    struct Token token;

    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *tree_generate(struct Lexer *lexer);
void tree_free(struct TreeNode *root);

void tree_print(struct TreeNode *root);
double tree_eval(struct TreeNode *root);

#endif
