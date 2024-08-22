#include <stdio.h>
#include <math.h>

#define Small_Diff 0.0001

//#if !defined(Online)
//#define Online

//int Function_That_Runs_Tests()
void Inputs_Of_Numbers(double *First_Ratio, double *Second_Ratio, double *Third_Ratio);

int Solve_Linear_Equation(double Second_Ratio, double Third_Ratio, double *Root_1);

int Solve_Squre_Equation(double First_Ratio, double Second_Ratio, double Third_Ratio, double *Root_1, double *Root_2);

void Contest(double First_Ratio, double Second_Ratio, double Third_Ratio, double Root_1_Expected, double Root_2_Expected, int Amount_Of_Roots_Expected);



int main(){
    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
    Inputs_Of_Numbers(&a, &b, &c);
    int nRoots = -1;


    if ( (a > Small_Diff) || (a < (- 1) *Small_Diff) )
        nRoots = Solve_Squre_Equation(a, b, c, &x1, &x2);

    else
        nRoots = Solve_Linear_Equation(b, c, &x1);

    switch (nRoots){
        case 3:
            printf("Endless amount of roots\n");
            break;

        case 2:
            printf("Root_1 = %lg, Root_2 = %lg, Amount_Of_Roots = %d\n", x1, x2, nRoots);
            break;

        case 1:
            printf("Root = %lg, Amount_Of_Roots = %d\n", x1, nRoots);
            break;

        case 0:
            printf("I`ve got no roots\n");
            break;

        default:
            printf("ERROR");
            break;


    }
}

void Inputs_Of_Numbers(double *a, double *b, double *c){
    double Input_Numbers[3];
    int result;

    printf("\t\tPlease type 3 different numbers:\n");

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
        printf("Ratio %d: %lf\n", i, Input_Numbers[i]);

    *a = Input_Numbers[0];
    *b = Input_Numbers[1];
    *c = Input_Numbers[2];
}

int Solve_Linear_Equation(double b, double c, double *x1){
    if( (b > Small_Diff) || (b < (- 1) * Small_Diff) ){
        *x1 = (-c / b);
        return 1;
        }
    else
        return 0;
}

int Solve_Squre_Equation(double a, double b, double c, double *x1, double *x2){
    double Discr = 0;
    Discr = (b * b) - (4 * a * c);
    printf("Discriminant: %lg \n", Discr);
    if (Discr > 0){
        *x1 = ( (-b) + sqrt(Discr) ) / (2 * a);
        *x2 = ( (-b) - sqrt(Discr) ) / (2 * a);
        return 2;
    }
    else if ( Discr < 0)
        return 3;
    else {
        *x1 =  (-b) / (2 * a);
        return 1;
    }
}


void Contest(double a, double b, double c, double x1_Expected, double x2_Expected, int nRoots_Expected){
    static int Test_Number = 1;
    double x1 = 0, x2 = 0;
    int nRoots = Solve_Squre_Equation(a, b, c, &x1, &x2);
    if ( (nRoots != nRoots_Expected) || (x1 > x1_Expected + Small_Diff) || (x1 < x1_Expected - Small_Diff)
                                     || (x2 > x2_Expected + Small_Diff) || (x2 < x2_Expected - Small_Diff) ){
        printf("Test ¹ %d Failed: a = %lg, b = %lg, c = %lg,  x1 = %lg, x2 = %lg, nRoots = %d\n", Test_Number, a, b, c, x1, x2, nRoots);
        printf("Expected: x1 = %lg, x2 = %lg, nRoots = %d\n", x1_Expected, x2_Expected, nRoots_Expected);
    }
        else
            printf("Test %d Succeeded", Test_Number);

    Test_Number += 1;

}


//#endif
