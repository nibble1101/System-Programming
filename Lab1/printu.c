#include<stdio.h>


typedef unsigned int u32;

char *tab = "0123456789ABCDEF";
int  BASE = 10; 

int rpu(u32 x)
{  
    char c;
    if (x){
       c = tab[x % BASE];
       rpu(x / BASE);
       putchar(c);
    }
}

int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

int main()
{
    u32 num;
    printf("Enter an unsigned integer number: ");
    scanf("%u", &num);
    printf("\n");
    printu(num);
    return 0;

}