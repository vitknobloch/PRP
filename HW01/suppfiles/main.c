#include <stdio.h>
#include <stdlib.h>

/* The main program */
int main(int argc, char *argv[])
{
  //init variables
  int num1, num2;
  //read input
  if(2 != scanf("%d %d", &num1, &num2)){
    printf("Vstup je chybný!\n");
  }

  //Check for interval
  if(num1 > 10000 || num2 > 10000 || num1 < -10000 || num2 < -10000){
    printf("Vstup je mimo interval!\n");
    return 0;
  }

  //Print in DEC & HEX
  printf("Desitkova soustava: %d %d\n", num1, num2);
  printf("Sestnactkova soustava: %x %x\n", num1, num2);

  //print addition, subtraction, multiplication
  printf("Soucet: %d + %d = %d\n", num1, num2, num1 + num2);
  printf("Rozdil: %d - %d = %d\n", num1, num2, num1 - num2);
  printf("Soucin: %d * %d = %d\n", num1, num2, num1 * num2);

  //division
  if(num2 == 0){
    printf("Nedefinovany vysledek!\n");
  }
  else{
    printf("Podil: %d / %d = %d\n", num1, num2, num1 / num2);
  }

  //average
  printf("Prumer: %.1f\n", (num1 + num2) / 2.0f);

  
  return 0;
}