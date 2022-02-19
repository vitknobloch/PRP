#include <stdio.h>
#include <stdlib.h>

#define INTERVAL_MAX 10000
#define INTERVAL_MIN -10000

int main(int argc, char *argv[])
{
  //initialize variables
  int latest_input;
  int inputs_count = 0;
  int sum = 0;
  int positive_count = 0;
  int negative_count = 0;
  int even_count = 0;
  int odd_count = 0;
  int max;
  int min;

  while(scanf("%d", &latest_input) == 1){ //read input while valid

    //check if input is from defined interval (in HW02 <-10 000, 10 000>)
    if(latest_input < INTERVAL_MIN || latest_input > INTERVAL_MAX){
      printf("\nError: Vstup je mimo interval!\n");
      return 100;
    }

    //print out lastest_input
    if(inputs_count > 0) //not first input
      printf(", %d", latest_input);
    else{ //first input
      printf("%d", latest_input);
      //if this is the first input also initialize min and max values
      min = latest_input;
      max = latest_input;
    }    

    //update sum and input counter
    sum += latest_input;
    inputs_count++;

    //count positive & negative
    if(latest_input > 0)
      positive_count++;
    else if(latest_input < 0)
      negative_count++;

    //count even & odd
    if(latest_input % 2 == 0)
      even_count++;
    else
      odd_count++;

    //update max and min
    if(latest_input > max)
      max = latest_input;
    if(latest_input < min)
      min = latest_input;

  }

  //Print output
  printf("\n");
  printf("Pocet cisel: %d\n", inputs_count);
  printf("Pocet kladnych: %d\n", positive_count);
  printf("Pocet zapornych: %d\n", negative_count);
  //percentage is (count positive / count all)*100
  printf("Procento kladnych: %.02f\n", 100*(positive_count/(float)inputs_count)); 
  printf("Procento zapornych: %.02f\n", 100*(negative_count/(float)inputs_count));
  printf("Pocet sudych: %d\n", even_count);
  printf("Pocet lichych: %d\n", odd_count);
  printf("Procento sudych: %.02f\n", 100*(even_count/(float)inputs_count));
  printf("Procento lichych: %.02f\n", 100*(odd_count/(float)inputs_count));
  //average is (sum all / count all)
  printf("Prumer: %.02f\n", sum/(float)inputs_count);
  printf("Maximum: %d\n", max);
  printf("Minimum: %d\n", min);
  
  return 0;
}
