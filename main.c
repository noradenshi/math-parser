#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 64

int eval(char *buffer) {
    size_t len = 0;
    while (isdigit(buffer[len]) && buffer[len] != 0) {
        len++;
    }

    char tmp = buffer[len];
    buffer[len] = 0;
    int l = atoi(buffer);
    buffer[len] = tmp;

    switch (buffer[len]) {
    case '+':
        return l + eval(buffer + len + 1);
    case '*':
        return l * eval(buffer + len + 1);
    default:
        return l;
    }
}

int main(void) {
    char buffer[BUFFER_SIZE];

    printf("equation: ");

    if (!fgets(buffer, BUFFER_SIZE, stdin))
        return EXIT_FAILURE;

    printf("eval: %d", eval(buffer));

    return EXIT_SUCCESS;
}
