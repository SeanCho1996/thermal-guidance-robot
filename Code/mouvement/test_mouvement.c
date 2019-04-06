#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mouvement.h"

main(){
FILE* uart;

uart = fopen("/dev/ttyO1","w");
av(uart);
ldr(uart);
lag(uart);
st(uart);

fclose(uart);
}