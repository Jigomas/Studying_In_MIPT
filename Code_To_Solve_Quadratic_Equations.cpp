#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

const int SMALL_DIFF  = 1e-8; //1e-x
enum Amount_Of_Roots
{
    QUADRATIC_EQUATION_ENDLESS_ROOTS =  3,
    QUADRATIC_EQUATION_TWO_ROOTS     =  2,
    QUADRATIC_EQUATION_ONE_ROOT      =  1,
    QUADRATIC_EQUATION_NO_ROOTS      =  0,
    QUADRATIC_EQUATION_ERROR         = -1,
};

//#if !defined(Online)
//#define Online

//int Function_That_Runs_Tests()



void Inputs_Of_Numbers(double *First_Ratio, double *Second_Ratio,
                       double *Third_Ratio);

int Solve_Linear_Equation(double Second_Ratio, double Third_Ratio,
                          double *Root_1);

int Solve_Square_Equation(double First_Ratio, double Second_Ratio,
                          double Third_Ratio, double *Root_1,
                          double *Root_2);

void Discr_Is_Positive(double Discr, double First_Ratio,
                       double Second_Ratio, double Third_Ratio,
                       double *Root_1, double *Root_2);

void Discr_Is_Zero(double First_Ratio, double Second_Ratio,
                   double *First_Root);

void Contest(double First_Ratio, double Second_Ratio,
             double Third_Ratio, double Root_1_Expected,
             double Root_2_Expected, int Amount_Of_Roots_Expected);



int main()
{
    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
    Inputs_Of_Numbers(&a, &b, &c);
    int Amount_Of_Roots = QUADRATIC_EQUATION_ERROR;

    if ( (a > SMALL_DIFF) || (a < -SMALL_DIFF) )
        Amount_Of_Roots = Solve_Square_Equation(a, b, c, &x1, &x2);

    else
        Amount_Of_Roots = Solve_Linear_Equation(b, c, &x1);

    switch (Amount_Of_Roots)
    {
        //  ENDLESS_ROOTS = 3, TWO_ROOTS = 2, ONE_ROOT = 1, NO_ROOTS = 0, ERROR = -1
        case QUADRATIC_EQUATION_ENDLESS_ROOTS:
            printf("Endless amount of roots\n");
            break;

        case QUADRATIC_EQUATION_TWO_ROOTS:
            printf("Root_1 = %lg, Root_2 = %lg, Amount_Of_Roots = %d\n", x1, x2, Amount_Of_Roots);
            break;

        case QUADRATIC_EQUATION_ONE_ROOT:
            printf("Root = %lg, Amount_Of_Roots = %d\n", x1, Amount_Of_Roots);
            break;

        case QUADRATIC_EQUATION_NO_ROOTS:
            printf("I`ve got no roots\n");
            break;

        default:
            printf("ERROR");
            break;
    }
}



void Inputs_Of_Numbers(double *a, double *b, double *c)
{
    assert (*a != NAN);
    assert (*b != NAN);
    assert (*c != NAN);
    assert (isfinite (*a) == 1);
    assert (isfinite (*b) == 1);
    assert (isfinite (*c) == 1);

    double Input_Numbers[3] = {0, 0, 0};

    printf("\t\tPlease type 3 different numbers which will be a Ratios of quadratic equation:\n");

    for (int i = 0; i < 3; i++)
    {
        while (1)
        {
            double Number = 0;
            int Result = 0, Current_Symbol = 0;

            printf("Number %d:\n", i + 1);
            Result = scanf("%lf", &Number);

            if (Result == 1)
            {
                while ( ((Current_Symbol = getchar() )  != '\n') && (Current_Symbol != EOF) )
                {
                    if ( isspace(Current_Symbol) == 0)
                    {
                        Result = 0;
                        break;
                    }
                }
                if (Result == 1)
                {
                    Input_Numbers[i] = Number;
                }
                else
                {
                    printf("Wrong number\n");
                    i = i - 1;
                    while (getchar() != '\n');
                }
                break;
            }
            else
            {
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



int Solve_Linear_Equation(double b, double c, double *x1)
{
    if( (b > SMALL_DIFF) || (b < -SMALL_DIFF) )
    {
        *x1 = (-c / b);
        return QUADRATIC_EQUATION_ONE_ROOT;
        }
    else
    {
        return QUADRATIC_EQUATION_NO_ROOTS;
    }
}



int Solve_Square_Equation(double a, double b,
double c, double *x1, double *x2)
{
    double Discr = 0;
    Discr = b * b - 4 * a * c;
    printf("Discriminant: %lg \n", Discr);
    if (Discr > SMALL_DIFF)
    {
        Discr_Is_Positive(Discr, a, b, c, x1, x2);
        return QUADRATIC_EQUATION_TWO_ROOTS;
    }
    else if ( Discr < -SMALL_DIFF)
        return QUADRATIC_EQUATION_ENDLESS_ROOTS;
    else
    {
        Discr_Is_Zero(a, b, x1);
        return QUADRATIC_EQUATION_ONE_ROOT;
    }
}



void Discr_Is_Positive(double Discr, double a, double b, double c, double *x1, double *x2)
{
    double Square_Root_From_Discr = sqrt(Discr);
    *x1 = (-b + Square_Root_From_Discr) / (2 * a);
    *x2 = (-b - Square_Root_From_Discr) / (2 * a);
}



void Discr_Is_Zero(double a, double b, double *x1)
{
    *x1 =  (-b) / (2 * a);
}



void Contest(double a, double b, double c,
            double x1_Expected, double x2_Expected, int Amount_Of_Roots_Expected)
{
    static int Test_Number = 1;
    double x1 = 0, x2 = 0;
    int Amount_Of_Roots = Solve_Square_Equation(a, b, c, &x1, &x2);
    if ( (Amount_Of_Roots != Amount_Of_Roots_Expected)
        || (x1 > x1_Expected + SMALL_DIFF)
        || (x1 < x1_Expected - SMALL_DIFF)
        || (x2 > x2_Expected + SMALL_DIFF)
        || (x2 < x2_Expected - SMALL_DIFF) )
    {
        printf("Test %d Failed: a = %lg, b = %lg, c = %lg,  x1 = %lg, x2 = %lg, Amount_Of_Roots = %d\n",
         Test_Number, a, b, c, x1, x2, Amount_Of_Roots);

        printf("Expected: x1 = %lg, x2 = %lg, Amount_Of_Roots = %d\n",
        x1_Expected, x2_Expected, Amount_Of_Roots_Expected);
    }
        else
            printf("Test %d Succeeded", Test_Number);

    Test_Number += 1;
}



//#endif
