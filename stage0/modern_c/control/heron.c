#include <tgmath.h>
#include <stdio.h>
#include <stdlib.h>

/* lower and upper iteration limits centered around 1.0 */
static double const eps1m01 = 1.0 - 0x1P-01;
static double const eps1p01 = 1.0 + 0x1P-01;
static double const eps1m24 = 1.0 - 0x1P-24;
static double const eps1p24 = 1.0 + 0x1P-24;

int main(int argc, char* argv[argc+1])
{
    printf("eps1mo1 = %g\n", eps1m01);
    for (int i = 1; i < argc; i++)
    {
        char *end;
        double const a = strtod(argv[i], &end);
        if (argv[i] != end)
        {
            double x = 1.0;

            for(;;)         // compute initial guess
            {
                printf("x=%.5e\n", x);
                double prod = a*x;
                if (prod < eps1m01)
                {
                    x *= 2.0;
                }
                else if (eps1p01 < prod)
                {
                    x *= 0.5;
                }
                else
                {
                    break;
                }
            }
            for (;;)        // Heron approximation
            {
                double prod = a*x;
                if ((prod < eps1m24) || (eps1p24 < prod))
                {
                    x *= (2.0 - prod);
                }
                else
                {
                    break;
                }
            }
            printf("heron: a=%.5e,\tx=%.5e,\ta*x=%.12f\n",
                    a, x, a*x);
        }
        else
        {
            printf("cannot convert %s into double.\n", argv[i]);
        }
    }

    return EXIT_SUCCESS;
}
