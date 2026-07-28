#include <stdio.h>
#define MAXMEM 30000

unsigned char MEM[MAXMEM];
unsigned char *current = MEM;

void init() {
    int i = 0;
    for(i = 0; i < MAXMEM; i++)
        MEM[i] = 0;
}

void increment() { (*current)++; }
void decrement() { (*current)--; }
void move_left() {
    unsigned char* right = MEM+MAXMEM;
    unsigned char* left = MEM;
    if(current == left) current = right;
    else current--;
}
void move_right() {
    unsigned char* right = MEM+MAXMEM;
    unsigned char* left = MEM;
    if(current == right-1) current = left;
    else current++;
}
void get_current() { *current = getchar(); }
void put_current() { putchar(*current); }