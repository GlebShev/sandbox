#include <stdio.h>
#include <setjmp.h>

jmp_buf begin; 
char curlex; 

void getlex(void); /* выделяет из входного потока очередную лексему */
int expr(void); /* распознает выражение и вычисляет его значение */
int add(void); /* распознает слагаемое и вычисляет его значение */
int mult(void); /* распознает множитель и вычисляет его значение */
int power(void); /* распознает возведение в степень и вычисляет его значение */
void error(void); /* сообщает об ошибке в выражении и передает управление в начало */

int main() {
    int result;
    setjmp(begin);
    printf("==>");
    getlex();
    result = expr();
    if (curlex != '\n') error();
    printf("\n%d\n", result);
    return 0;
}

void getlex() {
    while ((curlex = getchar()) == ' ');
}

void error(void) {
    printf("\nОШИБКА!\n");
    while (getchar() != '\n');
    longjmp(begin, 1);
}

int expr() {
    int e = add();
    while (curlex == '+' || curlex == '-') {
        char op = curlex;
        getlex();
        if (op == '+') {
            e += add();
        } else {
            e -= add();
        }
    }
    return e;
}

int add() {
    int a = mult();
    while (curlex == '*' || curlex == '/') {
        char op = curlex;
        getlex();
        if (op == '*') {
            a *= mult();
        } else {
            int divisor = mult();
            if (divisor == 0) error();
            a /= divisor;
        }
    }
    return a;
}

int mult() {
    int m = power();
    while (curlex == '^') {
        getlex();
        int exponent = power();
        int result = 1;
        for (int i = 0; i < exponent; i++) {
            result *= m;
        }
        m = result;
    }
    return m;
}

int power() {
    int m;
    switch (curlex) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            m = curlex - '0';
            break;
        case '(':
            getlex();
            m = expr();
            if (curlex == ')') break;
        default:
            error();
    }
    getlex();
    return m;
}

