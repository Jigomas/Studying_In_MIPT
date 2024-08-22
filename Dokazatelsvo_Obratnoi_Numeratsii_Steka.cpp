# include <stdio.h>
int main()
{
    int *a1;
    int *a2;
    int *a3;

    int b1 = 5;
    int b2 = 5;
    int b3 = 5;

    a1 = &b1;
    a2 = &b2;
    a3 = &b3;

    printf("a1 = %d a2 = %p a3 = %p   b1 = %d b2 = %d b3 = %d", *a1, a2, a3, b1, b2, b3);
}
