#include <stdio.h>

int main(void)
{
    double x;
    double k=0.0;
    double res = 0.0;
    scanf("%lf", &x);
    while ((scanf("%lf", &k)) == 1)
    {
        res = res*x + k;
    }
    printf("%lf", res);
    return 0;
}
