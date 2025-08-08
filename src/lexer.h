#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

// token type:
//  max for number
//  ascii for other
#define tok_number (char)255

struct Token {
    char type;
    int value;
};

void token_print(struct Token token);

struct Lexer {
    char *data;
    size_t offset;
};

void lexer_init(struct Lexer *lexer, char *data);
struct Token lexer_next(struct Lexer *lexer);

#endif
