#include <stdio.h>

int main(void){
    double x;
    double k;
    double res;
    double res2 = 0.0;
    double res3;
    double n;
    scanf("%lf", &n);
    scanf("%lf", &x);
    scanf("%lf", &k);
    res = k;
    res3 = x * (k/(n + 1));
    for (int i = n-1; i >= 0; i--){
        scanf("%lf", &k);
        res2 = res2*x + res;
        res = res*x + k;
        res3 = (res3 + k/(i + 1))*x;
    }
    printf("Многочлен:\n");
    printf("%lf\n", res);
    printf("Производная:\n");
    printf("%lf\n", res2);
    printf("Интеграл:\n");
    printf("%lf\n", res3);
    return 0;
}
