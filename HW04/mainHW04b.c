/*------BONUS TASK HW04b------*/
#include <stdio.h>
#include <stdlib.h>

#define MAX_PRIME 1000000
#define MAX_DIGITS_COUNT 100

/*Initialize numbers into the given field (index = number)*/
void initialize_eratosthenes_sieve(int sieve[], int limit){
  //initialize field for Sieve of Eratosthenes (cz: Eratosthenovo sito)
  sieve[0] = 0;
  sieve[1] = 0;
  for(int i = 2; i < limit; i++){
    sieve[i] = i;
  }
}

/*Runs the Eratosthenes sieve on the given field and returns the count of found primes*/
int run_eratosthenes_sieve(int sieve[], int limit){
  //run the Sieve and count how many primes have been found
  int primes_count = 0;
  for(int i = 0; i < limit; i++){
    //if the number wasn't crossed out (set to 0) before the cycle gets to it 
    //-> it's a prime
    if(sieve[i] != 0){ 
      primes_count++;
      for(int p = 2; p * sieve[i] < limit; p++){
        //cross out (set to 0) all multiples of the found prime to the limit MAX_PRIME
        sieve[sieve[i]*p] = 0;
      }
    }
  }
  return primes_count;
}

/*Extracts the primes from the sieve and saves them to the primes field*/
void get_primes_from_sieve(int sieve[], int primes[], int limit){
  int next_primes_index = 0;
  //iterate through the crossed out field of numbers
  for(int i = 0; i < limit; i++){
    if(sieve[i] != 0){
      //copy each NOT crossed out number to the primes field
      primes[next_primes_index] = sieve[i];
      next_primes_index++;
    }
  }
}

/*print the right power of prime to output in the right format*/
void print_prime(int prime, int prime_power, _Bool first_prime){
  if(first_prime){ //if it's not the first found prime
    printf(" x ");
  }
  printf("%d", prime);
  if(prime_power > 1){ //if the prime forms the input in power > 1
    printf("^%d", prime_power);
  }
}

/*divides large number digit by digit as a human*/
int divide_large(char large_int[], int divisor, int num_len, _Bool rewrite_large){
  //initialize properties
  int divident = 0;
  int remainder = 0;
  char result[num_len];

  int not_null_count = 0; //used to not copy zeros in the begining to the result
  int next_index = 0;
  while(large_int[next_index] != -1){
    //write down the next digit to the remainer from previous step
    divident = remainder * 10 + large_int[next_index];
    //save the remainder of this step for use in next step
    remainder = divident % divisor; 

    //in order to save a lot of computing only save the result if told to
    if(rewrite_large){ 
      int r_digit = divident / divisor;
      //dont write the zeroes in the begining to result
      if(not_null_count > 0 || r_digit != 0){ 
        //save the result of floor division of this step to result
        result[not_null_count] = r_digit;
        not_null_count++;
      }
        
    }
    next_index++;
  }
  //save the result to the input field
  if(rewrite_large){  //but only if told to
    for(int i = 0; i < not_null_count; i++){
      large_int[i] = result[i];
    }
    large_int[not_null_count] = -1;
  }
  return remainder; //return the remainder after last step
}

/*Changes each digit character to it's value (in place) 
and returns the lenght of the parsed number*/
int parse_large_int(char str[]){
  int len = 0;
  while (str[len] != '\0') {
    //thanks to ASCII order of the digits value of each digit is (digit - 0 digit char)
    str[len] -= '0'; 
    if(str[len] > 9 || str[len] < 0){ 
      //return error code -1 if any character is not a digit
      return -1;
    }
    len++;
  }
  //replace the end string char (\0) with -1 to prevent errors
  str[len] = -1;
  return len;
}

/*prints out prime factorization for any 'number' > 1)*/
void factorize_number(int primes[], int primes_count, char number[], int num_len){
  int primes_found = 0; //only used for output formatting
  
  //iterate through primes field starting at the lowest primes
  for(int i = 0; i<primes_count; i++){
    int prime_power = 0;
    //-> how many times did the prime divide the number

    //divide the number fast to just get the remainder
    while(divide_large(number, primes[i], num_len, 0) == 0){ 
      prime_power++;
      //just if the remainder is equal to zero:
      //divide the number again and this time save the result (in place)
      divide_large(number, primes[i], num_len, 1);
    }

    if(prime_power > 0){
      primes_found++;
      print_prime(primes[i], prime_power, primes_found > 1);

      //if '1' is all that's left from the input 
      // -> all the primes forming the input have been found
      if(number[1] == -1 && number[0] == 1){
        printf("\n");
        break;
      }
    }
  }
  //if the number still isn't one it has larger factors than the precalculated primes
  //the program could continue using the naive algorithm from here, 
  //but I decided to end it with partial output and error message
  if(number[1] != -1 || number[0] != 1){
    printf(" |Error: This number has at least one factor larger than %d\n", MAX_PRIME);
    fprintf(stderr, "Error: Too large prime factor error.");
  }
  
}

/*Copy either 'len' chracters of first string to the copy string or end at '\0'*/
void copy_string(char original[], char copy[], int len){
  for(int i = 0; i < len; i++){
    copy[i] = original[i];
    if(copy[i] == '\0'){
      break;
    }
  }
}

/* The main program */
int main(int argc, char *argv[])
{
  //FIND PRIMES TO THE GIVEN LIMIT
  int sieve[MAX_PRIME];
  initialize_eratosthenes_sieve(sieve, MAX_PRIME);
  int primes_count = run_eratosthenes_sieve(sieve, MAX_PRIME);  

  int primes[primes_count]; //field for the found primes
  get_primes_from_sieve(sieve, primes, MAX_PRIME);  
  //All the primes to the given limit are stored in 'primes[]'


  char input[MAX_DIGITS_COUNT + 1];
  while(scanf("%100s", input) == 1){ //main loop

    //copy the input for use in output
    char input_str_copy[MAX_DIGITS_COUNT + 1];
    copy_string(input, input_str_copy, MAX_DIGITS_COUNT + 1);

    //parse the large number as described in the method
    int num_len = parse_large_int(input); 
    //num_len stores the initial count of the number's digit

    //end the program successfully if input is '0'
    if(input[0] == 0 && input[1] == -1){
      return 0;
    }
    //end the programm with 100 return code if input is negative
    if(num_len < 0){
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    //output special output for input '1' and end this iteration of while cycle
    if(input[0] == 1 && input[1] == -1){
      printf("Prvociselny rozklad cisla 1 je:\n1\n");
      continue;
    }

    //find & print the factors for every other input
    printf("Prvociselny rozklad cisla %s je:\n", input_str_copy);
    factorize_number(primes, primes_count, input, num_len);
    
  }

  //The program only reaches this when invalid input was scanned
  fprintf(stderr, "Error: Chybny vstup!\n");
  return 100;
}

