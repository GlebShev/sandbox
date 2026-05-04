#include <stdio.h>
double str2double(char str[]){
    int i = 0;
    int sig = 1;
    double num = 0;
    double frac_ord = 0.1;
    int e_sig = 1;
    int e_mas = 0;
    
    
    while (str[i] == ' ' || str[i] == '\n'){ //если есть пробелы перед числом
        i++;
    } 
    
    if (str[i] == '-'){ // проверка знака
        sig = -1;
        i++;
    }else if(str[i] == '+'){
        i++;
    }
    
    while (str[i] <= '9' && str[i] >= '0'){ // целая
        num = num*10 + (str[i] - '0');
        i++;
    }
    if (str[i] == '.'){//дробная
        i++;
        while (str[i] <= '9' && str[i] >= '0'){
            num += (str[i] - '0')*frac_ord;
            frac_ord /= 10;
            i++;
        }
    }
    if (str[i] == 'e' || str[i] == 'E'){//экспоненциальная 
        i++;
        if (str[i] == '-'){//знак 
            e_sig = -1;
            i++;
        }else if(str[i] == '+'){
            i++;
        }
        while(str[i] <= '9' && str[i] >= '0'){//порядок
            e_mas = e_mas*10 + (str[i] - '0');
            i++;
        }
        while (e_mas > 0){//добавление к числу
            if (e_sig == 1){
                num *= 10;
            }else{
                num /= 10;
            }
            e_mas--;
        }
        
    }
    // мы выдаем в дабл, а лонг в задаче не нужен, поэтому обработки суффиксов нет
    return sig * num;
}


int main(){
    char str[50];//?
    while ((scanf("%s", str) == 1)){
        printf("%.10g\n", str2double(str));
    }
}
