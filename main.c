/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>
#include <string.h>

int putcmd(FILE*, char);
void init_program(FILE*);
void end_program(FILE*);

int main(int argc, char *argv[]) {
    FILE *input = NULL, *output = NULL;
    /** argument checking\ @todo make it support at max 4 args **/
    if(argc != 4) {
        fprintf(stderr, "usage: brfktoc file -o output.c");
        return 1;
    }
    int i;
    for(i = 1; i < argc; i++) {
        int is_o = strcmp(argv[i], "-o") == 0;
        /** when "-o" found between pos 1 and 2 **/ 
        if(i != 3 && is_o) {
            /** go to the next argument and open the file **/
            ++i;
            output = fopen(argv[i], "w");
            if(output == NULL) {
                if(input != NULL)
                    fclose(input);
                fprintf(stderr, "error: could not load output file\n");
                return 1;
            }
        }
        /** when "-o" is the last parameter **/
        else if(i == 3 && is_o) {
            if(input != NULL)
                fclose(input);
            fprintf(stderr, "error: output argument declared but never read\n");
        }
        else {
            /** an input file is already loaded **/
            if(input != NULL) {
                fclose(input);
                fprintf(stderr, "error: more than one input file declared\n");
                return 1;
            }
            input = fopen(argv[i], "r");
            if(input == NULL) {
                fprintf(stderr, "error: could not load input file\n");
                return 1;
            }
        }
    }

    /** translating the program **/
    init_program(output);
    int c;
    while((c = getc(input)) != EOF) {
        int puterr = putcmd(output, c);
        if(puterr > 0) {
            fclose(input);
            fclose(output);
            return puterr;
        }
    }
    end_program(output);

    fclose(input);
    fclose(output);
    return 0;
}
int putcmd(FILE* outfile, char c) {
    /** keeps track of the closing of the loops */
    static int loopstack = 0;
    switch(c) {
    case '<':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "move_left();\n");
        return 0;
    case '>':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "move_right();\n");
        return 0;
    case '.':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "put_current();\n");
        return 0;
    case ',':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "get_current();\n");
        return 0;
    case '+':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "increment();\n");
        return 0;
    case '-':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "decrement();\n");
        return 0;
    case '[':
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        loopstack++; // adds loop to the stack
        fprintf(outfile, "while(*current) {\n");
        return 0;
    case ']':
        if(loopstack == 0) {
            fprintf(stderr, "error: attempting to close uninitialized loop\n");
            return 1;
        }
        loopstack--; // removes loop
        for(int i = 0; i < loopstack+1; i++) fprintf(outfile, "\t");
        fprintf(outfile, "}\n");
        return 0;
    default:
        fprintf(stderr, "error: unrecognized command %c\n", c);
        return 1;
    }
}
void init_program(FILE* outfile) {
    fprintf(outfile, "#include <stdio.h>\n\
        #define MAXMEM 30000\n\
        unsigned char MEM[MAXMEM];\n\
        unsigned char *current = MEM;\n\
        void init() {\n\
        \tint i = 0;\n\
        \tfor(i = 0; i < MAXMEM; i++) MEM[i] = 0;\n\
        }\n\
        void increment() {(*current)++;}\n\
        void decrement() {(*current)--;}\n\
        void move_left() {\n\
        \tunsigned char* right = MEM+MAXMEM-1;\n\
        \tunsigned char* left = MEM;\n\
        \tif(current == left) current = right;\n\
        \telse current--;\n\
        }\n\
        void move_right() {\n\
        \tunsigned char* right = MEM+MAXMEM;\n\
        \tunsigned char* left = MEM;\n\
        \tif(current == right-1) current = left;\n\
        \telse current++;\n\
        }\n\
        void put_current() {putchar(*current);}\n\
        void get_current() { *current = getchar(); }\n\
        int main() {\n\
        \tinit();\n");
}
void end_program(FILE* outfile) {
    fprintf(outfile, "\treturn 0;\n");
    fprintf(outfile, "}");
}
