#include<stdio.h>


typedef unsigned int u32;

char *tab = "0123456789ABCDEF";
int  BASE = 16; 

int rpx(u32 x)
{  
    char c;
    
    if (x){
       c = tab[x % BASE];
       rpx(x / BASE);
       putchar(c);
    }
}

int printx(u32 x)
{
    putchar('0');
    putchar('x');
   (x==0)? putchar('0') : rpx(x);
   putchar(' ');
}

int main()
{
    u32 num;
    printf("Enter an unsigned integer number: ");
    scanf("%u", &num);
    printf("\n");
    printx(num);
    return 0;

}