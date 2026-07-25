/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>

void putcmd(char);
void init_program(void);
void end_program(void);

int main() {
    init_program();
    char c;
    while((c = getchar()) != EOF) {
        putcmd(c);
    }
    end_program();
    return 0;
}

void putcmd(char c) {
    switch(c) {
    case '<':
        printf("\tmove_left();\n");
        break;
    case '>':
        printf("\tmove_right();\n");
        break;
    case '.':
        printf("\tput_current();\n");
        break;
    case '+':
        printf("\tincrement();\n");
        break;
    case '-':
        printf("\tdecrement();\n");
        break;
    case '[':
        printf("\twhile(*current) {\n");
        // handle whats inside the loop
        break;
    case ']':
        printf("\t}\n");
        break;
    default:
        break;
    }
}

void init_program(void) {
    printf("#include <stdio.h>\n");
    printf("#define MAXMEM 300000\n");
    printf("unsigned char MEM[MAXMEM];\n");
    printf("unsigned char *current = MEM;\n");
    printf("void init() {\n");
    printf("\tint i = 0;\n");
    printf("\tfor(i = 0; i < MAXMEM; i++) MEM[i] = 0;\n");
    printf("}\n");
    printf("void increment() {(*current)++;}\n");
    printf("void decrement() {(*current)--;}\n");
    printf("void move_left() {\n");
    printf("\tunsigned char* right = MEM+MAXMEM;\n");
    printf("\tunsigned char* left = MEM;\n");
    printf("\tif(current == left) current = right;\n");
    printf("\telse current--;\n");
    printf("}\n");
    printf("void move_right() {\n");
    printf("\tunsigned char* right = MEM+MAXMEM;\n");
    printf("\tunsigned char* left = MEM;\n");
    printf("\tif(current == right-1) current = left;\n");
    printf("\telse current++;\n");
    printf("}\n");
    printf("void put_current() {putchar(*current);}\n");
    printf("int main() {\n");
    printf("\tinit();\n");
}

void end_program(void) {
    printf("\treturn 0;\n");
    printf("}");
}
