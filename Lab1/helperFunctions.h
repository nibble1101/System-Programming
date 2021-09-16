#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H




typedef unsigned int u32;

char *tab = "0123456789ABCDEF";
int  BASE = 10; 


//PRINTING THE UNSIGNED INTEGERS

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



//PRINTING THE STRINGS

void prints(char *s)
{
    while(*s != '\0')
    {
        printf("%c", *s);
        s++;
    }
}


//PRINTING THE INTEGERS

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

//PRINTING THE HEXADECIMALS

int rpx(u32 x)
{  
    BASE = 16;
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

//PRINTING THE OCTAL

int rpo(u32 x)
{  
    char c;
    
    BASE = 8;
    
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


void myprintf(char *fmt, ...)
{
    int *ip, *ebp = NULL;
    ebp = (int *)getebp();
    ip = ebp + 3;
    if(strcmp(fmt,"") == 0)
    {
        return;
    }
    else
    {
        int i = 0;
        while(fmt[i]!='\0')
        {
            if(fmt[i] == '%')
            {
                i+=1;
                switch (fmt[i])
                {
                case 'x':
                    printx(*ip);
                    ip++;
                    i+=1;
                    break;

                case 'd':
                    printd(*ip);
                    ip++;
                    i+=1;
                    break;

                case 'o':
                    printo(*ip);
                    ip++;
                    i+=1;
                    break;
                
                case 's':
                    prints(*ip);
                    ip++;
                    i+=1;
                    break;
                
                case '%':
                    putchar('%');
                    break;
                
                default:
                    break;
                }
            }

            else if(fmt[i] == '/')
            {
                i++;
                if(fmt[i] == 'n')
                {
                    putchar('\n');
                    putchar('\r');
                }
            }
            else
            {
                putchar(fmt[i]);
                i++;
            }
                
        }
    }

}

#endif