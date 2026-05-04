#include <stdio.h>
int main(){
	for (char i='a'; i<='z'; i++)
		printf("%c -  %d\n", i, i);
	for (char i='A'; i<='Z'; i++)
                printf("%c -  %d\n", i, i);
	for (char i='0'; i<='9'; i++)
                printf("%c -  %d\n", i, i);
	printf("Пробел - %d\n", ' ');
	printf("%d\n", '0'+'Z');
}

