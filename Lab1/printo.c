#include<stdio.h>


typedef unsigned int u32;

char *tab = "0123456789ABCDEF";
int  BASE = 8; 

int rpo(u32 x)
{  
    char c;
    
    if (x){
       c = tab[x % BASE];
       rpo(x / BASE);
       putchar(c);
    }
}

int printo(u32 x)
{
    putchar('0');
   (x==0)? putchar('0') : rpo(x);
   putchar(' ');
}

int main()
{
    u32 num;
    printf("Enter an unsigned integer number: ");
    scanf("%u", &num);
    printf("\n");
    printo(num);
    return 0;

}