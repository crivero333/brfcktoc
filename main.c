/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>
#include <string.h>

FILE *input;
FILE *output;

int putcmd(char);
void init_program(void);
void end_program(void);

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("usage: bfktoc -i source -o output.c");
        return 1;
    }
    
    int i;
    for(i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-i") == 0) {
            ++i;
            input = fopen(argv[i], "r");
            if(input == NULL) {
                fprintf(stderr, "error loading input file\n");
                return 1;
            }
        }
        else if(strcmp(argv[i], "-o") == 0) {
            ++i;
            output = fopen(argv[i], "w");
            if(output == NULL) {
                fprintf(stderr, "error loading output file\n");
                return 1;
            }
        }
    }

    // constructing the final program
    init_program();
    int c;
    while((c = getc(input)) != EOF) {
        int puterr = putcmd(c);
        if(puterr > 0) {
            fclose(input);
            fclose(output);
            return puterr;
        }
    }
    end_program();

    fclose(input);
    fclose(output);
    return 0;
}
int putcmd(char c) {
    static int loopstack = 0;
    switch(c) {
    case '<':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "move_left();\n");
        return 0;
    case '>':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "move_right();\n");
        return 0;
    case '.':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "put_current();\n");
        return 0;
    case ',':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "get_current();\n");
        return 0;
    case '+':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "increment();\n");
        return 0;
    case '-':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "decrement();\n");
        return 0;
    case '[':
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        loopstack++; // adds loop to the stack
        fprintf(output, "while(*current) {\n");
        return 0;
    case ']':
        if(loopstack == 0) {
            fprintf(stderr, "error: attempting to close uninitialized loop\n");
            return 1;
        }
        loopstack--; // removes loop
        for(int i = 0; i < loopstack+1; i++) fprintf(output, "\t");
        fprintf(output, "}\n");
        return 0;
    default:
        fprintf(stderr, "error: unrecognized command %c\n", c);
        return 1;
    }
}
void init_program(void) {
    fprintf(output, "#include <stdio.h>\n");
    fprintf(output, "#define MAXMEM 30000\n");
    fprintf(output, "unsigned char MEM[MAXMEM];\n");
    fprintf(output, "unsigned char *current = MEM;\n");
    fprintf(output, "void init() {\n");
    fprintf(output, "\tint i = 0;\n");
    fprintf(output, "\tfor(i = 0; i < MAXMEM; i++) MEM[i] = 0;\n");
    fprintf(output, "}\n");
    fprintf(output, "void increment() {(*current)++;}\n");
    fprintf(output, "void decrement() {(*current)--;}\n");
    fprintf(output, "void move_left() {\n");
    fprintf(output, "\tunsigned char* right = MEM+MAXMEM-1;\n");
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
    fprintf(output, "void get_current() { *current = getchar(); }\n");
    fprintf(output, "int main() {\n");
    fprintf(output, "\tinit();\n");
}
void end_program(void) {
    fprintf(output, "\treturn 0;\n");
    fprintf(output, "}");
}
