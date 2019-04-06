#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void delay(int i)
{
int s;
int milli_second = 1000 * i;
clock_t start_time = clock();
for(s = 0; s<i; s++);
while(clock() < start_time + milli_second);
}


void av(FILE* uart)
{
char *str;

str = "av";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(10000);
}


void ar(FILE* uart)
{
char* str;

str = "ar";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
}

void st(FILE* uart)
{
char* str;

str = "st";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
}

void ldr(FILE* uart)
{
char* str;

str = "ldr";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void mdr(FILE* uart)
{
char* str;

str = "mdr";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void rdr(FILE* uart)
{
char* str;

str = "rdr";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void lga(FILE* uart)
{
char* str;

str = "lga";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void mga(FILE* uart)
{
char* str;

str = "mga";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void rga(FILE* uart)
{
char* str;

str = "rga";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void lad(FILE* uart)
{
char* str;

str = "lad";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void mad(FILE* uart)
{
char* str;

str = "mad";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void rad(FILE* uart)
{
char* str;

str = "rad";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void lag(FILE* uart)
{
char* str;

str = "lag";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void mag(FILE* uart)
{
char* str;

str = "mag";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}

void rag(FILE* uart)
{
char* str;

str = "rag";
fprintf(uart,"%s\n",str);
printf("%s\n",str);
delay(5000);
}