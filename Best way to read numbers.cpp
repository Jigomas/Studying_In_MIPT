#include <stdio.h>
void Inputs_Of_Numbers(double *First_rRatio, double *Second_Ratio, double *Third_Ratio);

int main(){
    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
    Inputs_Of_Numbers(&a, &b, &c);
    printf("%lg   %lg   %lg", a, b, c);

}

void Inputs_Of_Numbers(double *a, double *b, double *c){
    double Input_Numbers[3];
    int result;

    printf("\t\tPlease type 3 different numbers:\n");

    for (int i = 0; i < 3; i++){
        while (1){
            printf("Number %d:\n\t", i + 1);
            result = scanf("%lf", &Input_Numbers[i]);

            if (result == 1)
                break;

            else{
                printf("Wrong number\n");
                while (getchar() != '\n');

            }
        }
    }

    printf("I`ve got:\n");

    for (int i = 0; i < 3; i++)
        printf("%lf\n", Input_Numbers[i]);

    *a = Input_Numbers[0];
    *b = Input_Numbers[1];
    *c = Input_Numbers[2];
}

