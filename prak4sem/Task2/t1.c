#include <stdio.h>

double sqr(double x, double eps){
    double x_i = 1.0;
    double x_n;
    while(1){
        x_n = 0.5 * (x_i + x/x_i);
        if (((x_i - x_n) < eps) && ((x_n - x_i) < eps)){
            break;
        }
        x_i = x_n;
    }
    return x_n;
}

int main(){
    double eps;
    double x;
    scanf("%lf", &eps);
    while(scanf("%lf", &x) == 1){
        printf("%.10g\n", sqr(x, eps));
    }
    return 0;
}
