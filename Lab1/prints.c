#include<stdio.h>

void prints(char *s);

int main()
{
    char str[100] = {'\0'};
    printf("Enter a string: ");
    gets(str);
    printf("\nThe string entered: ");
    prints(str);
    printf("\n");
    return 0;
}

void prints(char *s)
{
    while(*s != '\0')
    {
        printf("%c", *s);
        s++;
    }
}