/**
 * @file main.c
 * @author crivero333
 * @brief transpiler of brainfuck source to c source
 */

#include <stdio.h>
#include <string.h>

int putcmd(FILE*, char);
void init_program(FILE*, FILE*);
void end_program(FILE*);

int main(int argc, char *argv[]) {
    FILE *input = NULL, *output = NULL;
    
    /** only one parameter, for input and output **/
    if(argc == 2) {
        input = fopen(argv[1], "r");
        if(input == NULL) {
            fprintf(stderr, "error: could not load the input file\n");
            return 1;
        }
        output = fopen(argv[1], "w+");
    }
    /** all parameters possible **/
    else if(argc == 4) {
        int i;
        for(i = 1; i < argc; i++) {
            int is_o = strcmp(argv[i], "-o") == 0;
            /** when "-o" found between pos 1 and 2 **/ 
            if(i != 3 && is_o) {
                /** go to the next argument and open the file, correct **/
                ++i;
                output = fopen(argv[i], "w+");
                if(output == NULL) {
                    if(input != NULL)
                        fclose(input);
                    fprintf(stderr, "error: could not load output file\n");
                    return 1;
                }
            }
            /** when "-o" is the last parameter, bad **/
            else if(i == 3 && is_o) {
                if(input != NULL)
                    fclose(input);
                fprintf(stderr, "error: output argument declared but never read\n");
                return 1;
            }
            else {
                /** an input file is already loaded, bad **/
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
    }
    /** missing parameters **/
    else {
        fprintf(stderr, "\
            usage: brfcktoc input_file\n\
                   brfcktoc input_file -o output_file.c\n");
        return 1;
    }

    FILE *headerfile = NULL;
    /** loading header file containing logic, somehow actual header file can lead to errors? **/
    headerfile = fopen("brfck_logic.c", "r");
    if(headerfile == NULL) {
        fprintf(stderr, "error: could not load header file\n");
        return 1;
    }
    /** translating the program **/
    init_program(output, headerfile);
    fclose(headerfile);
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
void init_program(FILE* outfile, FILE* logicheader) {
    /**
    @todo have to find some efficient way to copy the contents
          from the logicheader to the outfile. ideally in a
          direct manner
    */
    char initbuffer[630];
    fscanf(logicheader, "%s", initbuffer);
    fprintf(outfile, "%s\n\
        int main() {\n\
        \tinit();\n", initbuffer);
}
void end_program(FILE* outfile) {
    fprintf(outfile, "\treturn 0;\n");
    fprintf(outfile, "}");
}
