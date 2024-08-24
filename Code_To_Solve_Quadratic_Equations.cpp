/**
* @param a is a First_Coefficient  of the equation
* @param b is a Second_Coefficient of the equation
* @param c is a Third_Coefficient  of the equation
*
* @param x1 is a First_Root  of the equation
* @param x2 is a Second_Root of the equation
*
* @return Number_Is_Received - checking if received string is a number
* @return Inputs_Of_Numbers - makes a massive of received numbers
* @return Inputs_Of_Numbers - makes a massive of received numbers
* @return Solve_Linear_Equation - solves equation if First_Coefficient is 0
* @return Solve_Quadric_Equation - solves equation if First_Coefficient is not 0
* @return Discr_Is_Positive - solves equation with 2 roots //todo all functions

*/


#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

const int SMALL_DIFF  = 1e-8;

enum Different_Amount_Of_Roots_By_Exit_Codes
{
    QUADRATIC_EQUATION_ERROR         = -1,
    QUADRATIC_EQUATION_NO_ROOTS      =  0,
    QUADRATIC_EQUATION_ONE_ROOT      =  1,
    QUADRATIC_EQUATION_TWO_ROOTS     =  2,
    QUADRATIC_EQUATION_ENDLESS_ROOTS =  3,
};


enum All_Possible_Exit_Codes
{
    SOMETHING_WENT_WRONG      = -1,
    PROGRAM_STILL_IN_PROGRESS =  0,
    ALL_FINISHED_SUCCESFULLY  =  1,
};



//#if !defined(Online)
//#define Online

//int Function_That_Runs_Tests()

struct Types_Of_Coefficients
{
    double a, b, c;
    double x1_Expected, x2_Expected;
    int Amount_Of_Roots_Expected;
};

int Solving_Quadratic_Or_Linear_Equation(int *Current_Code_Of_Program);

int Return_Codes_Of_Program(double x1, double x2, int Amount_Of_Roots_Of_Quadric_Equation);

int Number_Is_Received(double *Input_Numbers, int i);

int Check_String_Empty(int Current_Symbol, int Result);

void Clear_Buf(int Current_Symbol);

void Inputs_Of_Numbers(double *First_Coefficient, double *Second_Coefficient,
                       double *Third_Coefficient);

int Solve_Linear_Equation(double Second_Coefficient, double Third_Coefficient,
                          double *Root_1);

int Solve_Quadric_Equation(double First_Coefficient, double Second_Coefficient,
                          double Third_Coefficient, double *Root_1,
                          double *Root_2);
//todo rename Discr
void Discr_Is_Positive(double Discr, double First_Coefficient,
                       double Second_Coefficient, double Third_Coefficient,
                       double *Root_1, double *Root_2);

int Discr_Is_Zero(double First_Coefficient, double Second_Coefficient,
                   double *First_Root);

void All_Tests();

void Test_With_Structures(Types_Of_Coefficients *Taken_Data, double *x1_Test, double *x2_Test);


void Test(double First_Coefficient, double Second_Coefficient,
             double Third_Coefficient, double Root_1_Expected,
             double Root_2_Expected, int Amount_Of_Roots_Expected);



int main()
{
    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
    //do while (Program_Return_Codes != IN_PROGRESSSION)
    int Current_Code_Of_Program = PROGRAM_STILL_IN_PROGRESS;
    int Amount_Of_Roots_Of_Quadric_Equation = 0;
    Amount_Of_Roots_Of_Quadric_Equation = Solving_Quadratic_Or_Linear_Equation(&Current_Code_Of_Program);
    Return_Codes_Of_Program(x1, x2,Amount_Of_Roots_Of_Quadric_Equation);
}


int Solving_Quadratic_Or_Linear_Equation(int *Current_Code_Of_Program)
{
    int Amount_Of_Roots_Of_Quadric_Equation = 0;
    while (*Current_Code_Of_Program == PROGRAM_STILL_IN_PROGRESS)
        {
            double a = 0, b = 0, c = 0, x1 = 0, x2 = 0; // todo to struct
            Inputs_Of_Numbers(&a, &b, &c);
            int Amount_Of_Roots_Of_Quadric_Equation = QUADRATIC_EQUATION_ERROR;

            if ( 0 != fabs(a) )
                Amount_Of_Roots_Of_Quadric_Equation = Solve_Quadric_Equation(a, b, c, &x1, &x2);

            else
                Amount_Of_Roots_Of_Quadric_Equation = Solve_Linear_Equation(b, c, &x1);

            *Current_Code_Of_Program = Return_Codes_Of_Program(x1, x2, Amount_Of_Roots_Of_Quadric_Equation);
        }
    return Amount_Of_Roots_Of_Quadric_Equation;
}


int Return_Codes_Of_Program(double x1, double x2, int Amount_Of_Roots_Of_Quadric_Equation)
{
    switch (Amount_Of_Roots_Of_Quadric_Equation)
        {
            //  ENDLESS_ROOTS = 3, TWO_ROOTS = 2, ONE_ROOT = 1, NO_ROOTS = 0, ERROR = -1
            case QUADRATIC_EQUATION_ENDLESS_ROOTS:
                printf("Endless amount of roots\n");

                return ALL_FINISHED_SUCCESFULLY;
                break;

            case QUADRATIC_EQUATION_TWO_ROOTS:
                printf("Root_1 = %lg, Root_2 = %lg, Amount_Of_Roots_Of_Quadric_Equation = %d\n", x1, x2, Amount_Of_Roots_Of_Quadric_Equation);

                return ALL_FINISHED_SUCCESFULLY;
                break;

            case QUADRATIC_EQUATION_ONE_ROOT:
                printf("Root = %lg, Amount_Of_Roots_Of_Quadric_Equation = %d\n", x1, Amount_Of_Roots_Of_Quadric_Equation);

                return ALL_FINISHED_SUCCESFULLY;
                break;

            case QUADRATIC_EQUATION_NO_ROOTS:
                printf("Quadratic equation have no roots\n");

                return ALL_FINISHED_SUCCESFULLY;
                break;

            default:
                printf("ERROR: No_Exit_Code_Received");

                return SOMETHING_WENT_WRONG;
                break;
        }
}



int Number_Is_Received(double *Input_Numbers, int i)
{
    double Number = 0;
    int Result = 0, Current_Symbol = 1;

    Result = scanf("%lf", &Number);

    Result = Check_String_Empty(Current_Symbol, Result);

    if (Result == 1)
    {
        *Input_Numbers = Number;

        return 1;
    }
    else
    {
        printf("Wrong number\n");

        Clear_Buf(Current_Symbol);
    }
    return 0;
}



int Check_String_Empty(int Current_Symbol, int Result)
{
    while (Result == 1 && Current_Symbol != '\n' && Current_Symbol != EOF)
    {
        Current_Symbol = getchar();
        if (isspace(Current_Symbol) == 0)
            Result = 0;
    }
    return Result;
}



void Clear_Buf(int Current_Symbol)
{
    while (Current_Symbol != '\n' && Current_Symbol != EOF)
    {
        Current_Symbol = getchar();
    }
    return;
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

    printf("\t\tPlease type 3 different numbers which will be a Coefficients of quadratic equation:\n");

    for (int i = 0; i < 3; i++)
    {
        printf("Number %d:\n", i + 1);
        int Is_Number_Received = 0;

        while (Is_Number_Received == 0)
        {
            Is_Number_Received = Number_Is_Received(&Input_Numbers[i],  i);
        }
    }

    printf("I`ve got:\n");

    for (int i = 0; i < 3; i++)
        printf("Coefficient %d: %lf\n", i + 1, Input_Numbers[i]);

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



int Solve_Quadric_Equation(double a, double b,
                          double c, double *x1, double *x2)
{
    double Discr = b * b - 4 * a * c;

    printf("Discriminant: %lg \n", Discr);

    if (Discr > SMALL_DIFF)
    {
        Discr_Is_Positive(Discr, a, b, c, x1, x2);

        return QUADRATIC_EQUATION_TWO_ROOTS;
    }
    else if (Discr < -SMALL_DIFF)
    {
        return QUADRATIC_EQUATION_ENDLESS_ROOTS;
    }
    else
    {
        if (Discr_Is_Zero(a, b, x1))
            return QUADRATIC_EQUATION_ONE_ROOT;
    }
}



void Discr_Is_Positive(double Discr, double a, double b, double c, double *x1, double *x2)
{
    double Quadric_Root_From_Discr = sqrt(Discr);
    *x1 = (-b + Quadric_Root_From_Discr) / (2 * a);
    *x2 = (-b - Quadric_Root_From_Discr) / (2 * a);
}



int Discr_Is_Zero(double a, double b, double *x1)
{
    *x1 =  (-b) / (2 * a);

    return 1;
}



/*void All_Tests()
{
    const int AMOUNT_OF_TESTS = 2;
                                       //  a, b, c, x1, x2, Amount_Of_Roots_Of_Quadric_Equation
    struct Types_Of_Coefficients Taken_Data [AMOUNT_OF_TESTS] = {{1, 2, 1, -1, 0, 1}, {2, 4, 2, -1, 0, 1}};
    for (int i = 0; i < AMOUNT_OF_TESTS; i++)
    {
        double x1_Test = 0, x2_Test = 0;
        Test_With_Structures(Taken_Data, &x1_Test, &x2_Test);
    }
}
*/


void Test_With_Structures(Types_Of_Coefficients *data, double *x1_Test, double *x2_Test)
{
    Solve_Quadric_Equation((*data).a, (*data).b, (*data).c, x1_Test, x2_Test);
}



void Test(double a, double b, double c,
             double x1_Expected, double x2_Expected, int Amount_Of_Roots_Expected)
{
    static int Test_Number = 1;
    double x1 = 0;
    double x2 = 0;

    int Amount_Of_Roots_Of_Quadric_Equation = Solve_Quadric_Equation(a, b, c, &x1, &x2);

    if ( (Amount_Of_Roots_Of_Quadric_Equation != Amount_Of_Roots_Expected)
        || (x1 > x1_Expected + SMALL_DIFF)
        || (x1 < x1_Expected - SMALL_DIFF)
        || (x2 > x2_Expected + SMALL_DIFF)
        || (x2 < x2_Expected - SMALL_DIFF) )
    {
        printf("Test %d Failed: a = %lg, b = %lg, c = %lg, x1 = %lg, x2 = %lg,"
            " Amount_Of_Roots_Of_Quadric_Equation = %d\n",Test_Number, a, b, c, x1, x2, Amount_Of_Roots_Of_Quadric_Equation);

        printf("Expected: x1 = %lg, x2 = %lg, Amount_Of_Roots_Of_Quadric_Equation = %d\n",
        x1_Expected, x2_Expected, Amount_Of_Roots_Expected);
    }
    else
    {
        printf("Test %d Succeeded", Test_Number);
    }

    Test_Number += 1;
}



//#endif
