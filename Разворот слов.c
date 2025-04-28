#include <stdio.h>
#include <ctype.h>
#include <stdlib.h> // For exit()


#define push(s_p, number) (*((s_p)++) = (number))
#define pop(s_p) (*--(s_p))

void process_input();
void reversing(char *s_p, char *stack, int *code);
void dump_stack (char **s_p, char **stack);



void reversing(char *s_p, char *stack, int *code) {
    if (code) {
        char c = 'a';
        
        while ((isspace(c) != 0) || (c != EOF) || (*code == 1)) {
            if (*code == 0) {
                return;
            }

            c = getchar();
            
            if (c == EOF) {
                dump_stack (&s_p, &stack);
                putchar('\n');
                *code = 0;

                return;
            }

            if (isspace(c)) {
                dump_stack (&s_p, &stack);
                putchar(' ');
                reversing(s_p, stack, code);
                
                exit;
                return;
            }
            push(s_p, c);
        }
    }
    return;

}


void dump_stack (char **s_p, char **stack){
    while (*s_p != *stack) putchar(pop(*s_p));
}


void process_input() {
    char stack[1000];
    char *s_p;
    s_p = stack;
    int code = 1;
    reversing(s_p, stack, &code);
}

int main() {
    process_input();
    printf("It`s all reversed\n");
}