#include <stdio.h>

int main(void){
    double x;
    double k;
    double res;
    double res2 = 0.0;
    scanf("%lf", &x);
    scanf("%lf", &k);
    res = k;
    while ((scanf("%lf", &k)) == 1){
        res2 = res2*x +res;
	res = res*x + k;
    }
    printf("Многочлен:\n");
    printf("%lf\n", res);
    printf("Производная:\n");
    printf("%lf\n", res2);
    return 0;
}

