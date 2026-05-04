#include <stdio.h>
int iter(int x){
    int i_p = 0;
    int i_n = 1;
    int fib = 0;
    if (x == 0) return 0;
    if (x == 1) return 1;
    for (int i = 2; i <= x; ++i){
        fib = i_p + i_n;
        i_p = i_n;
        i_n = fib;
    }
    return fib;
}

int rec(int x){
    if (x == 0){
        return 0;
    }else if (x == 1){
        return 1;
    } else{
        return rec(x-1) + rec(x - 2);
    }
}

int main(){
    int x;
    while(scanf("%d", &x)==1){
        printf("Итерация %d\n", iter(x));
        printf("Рекурсия %d\n", rec(x));
    }
}
