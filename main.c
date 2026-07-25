/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>
void putcmd(char c) {
    switch(c) {
    case '<':
        printf("move_left();\n");
        break;
    case '>':
        printf("move_right();\n");
        break;
    case '.':
        printf("put_current();\n");
        break;
    case '+':
        printf("increment();\n");
        break;
    case '-':
        printf("decrement();\n");
        break;
    case '[':
        printf("while(*current) {\n");
        // handle whats inside the loop
        break;
    case ']':
        printf("}\n");
        break;
    default:
        break;
    }
}

int main() {
    char c;
    while((c = getchar()) != EOF) {
        putcmd(c);
    }

    return 0;
}