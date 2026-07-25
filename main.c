/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>
#include <string.h>

FILE *input;
FILE *output;

void putcmd(char);
void init_program(void);
void end_program(void);

int main(int argc, char *argv[]) {
    //input = fopen("../ex.txt", "r");
    //output = fopen("./test.c", "w");

    if(argc != 5) {
        printf("usage: bfktoc -i source -o output.c");
        return 1;
    }
    
    int i;
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) {
            ++i;
            input = fopen(argv[i], "r");
        }
        else if(strcmp(argv[i], "-o") == 0) {
            ++i;
            output = fopen(argv[i], "w");
        }
    }

    init_program();
    char c;
    while((c = getc(input)) != EOF) {
        putcmd(c);
    }
    end_program();
    return 0;

    fclose(input);
    fclose(output);
    return 0;
}
void putcmd(char c) {
    switch(c) {
    case '<':
        fprintf(output, "\tmove_left();\n");
        break;
    case '>':
        fprintf(output, "\tmove_right();\n");
        break;
    case '.':
        fprintf(output, "\tput_current();\n");
        break;
    case '+':
        fprintf(output, "\tincrement();\n");
        break;
    case '-':
        fprintf(output, "\tdecrement();\n");
        break;
    case '[':
        fprintf(output, "\twhile(*current) {\n");
        // handle whats inside the loop
        break;
    case ']':
        fprintf(output, "\t}\n");
        break;
    default:
        break;
    }
}
void init_program(void) {
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#define MAXMEM 300000\n");
    fprintf(output, "unsigned char MEM[MAXMEM];\n");
    fprintf(output, "unsigned char *current = MEM;\n");
    fprintf(output, "void init() {\n");
    fprintf(output, "\tint i = 0;\n");
    fprintf(output, "\tfor(i = 0; i < MAXMEM; i++) MEM[i] = 0;\n");
    fprintf(output, "}\n");
    fprintf(output, "void increment() {(*current)++;}\n");
    fprintf(output, "void decrement() {(*current)--;}\n");
    fprintf(output, "void move_left() {\n");
    fprintf(output, "\tunsigned char* right = MEM+MAXMEM;\n");
    fprintf(output, "\tunsigned char* left = MEM;\n");
    fprintf(output, "\tif(current == left) current = right;\n");
    fprintf(output, "\telse current--;\n");
    fprintf(output, "}\n");
    fprintf(output, "void move_right() {\n");
    fprintf(output, "\tunsigned char* right = MEM+MAXMEM;\n");
    fprintf(output, "\tunsigned char* left = MEM;\n");
    fprintf(output, "\tif(current == right-1) current = left;\n");
    fprintf(output, "\telse current++;\n");
    fprintf(output, "}\n");
    fprintf(output, "void put_current() {putchar(*current);}\n");
    fprintf(output, "int main() {\n");
    fprintf(output, "\tinit();\n");
}
void end_program(void) {
    fprintf(output, "\treturn 0;\n");
    fprintf(output, "}");
}
