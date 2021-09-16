#include<stdio.h>

char *tab = "0123456789ABCDEF";
int  BASE = 10; 

int rpd(int x)
{  
    char c;
    if (x){
       c = tab[x % BASE];
       rpd(x / BASE);
       putchar(c);
    }
}

int printd(int x)
{
    if(x < 0)
    {
        putchar('-');
        x*=-1;
    }
   (x==0)? putchar('0') : rpd(x);
   putchar(' ');
}

int main()
{
    int num;
    printf("Enter an unsigned integer number: ");
    scanf("%d", &num);
    printf("\n");
    printd(num);
    return 0;

}