#include <stdio.h>
#include <math.h>
#include <stdlib.h>

double beta(double alpha);
double fib(int n, double x1, double x2, double al);
int main(int argc, char * argv[])
{
    if (argc > 1)
    {
        double w = atof(argv[1]) / 2;
        double alpha = 2 * cos(w);
        FILE * data = fopen("data.dat", "w");
        for (int i = 1; i < 10000; i++)
            fprintf(data, "%lg\n", fib(i, 1 * alpha, 1 * beta(alpha), alpha));
        fclose(data);
        return 1;
    }
    fprintf(stderr,"<./run.out> <alpha>");
    return 0;


}

double beta(double alpha)
{
    // return sqrt(1 - ((alpha * alpha) / 4));
    return -1;
}

double fib(int n, double x1, double x2, double al)
{
    if (n == 0) return x1;
    return fib(n - 1, al * x1 + beta(al) * x2, x1, al);
}
