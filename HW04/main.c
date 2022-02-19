#include <stdio.h>
#include <stdlib.h>

#define MAX_PRIME 1000000

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

/*prints out prime factorization for any 'number' > 1)*/
void factorize_number(int primes[], int primes_count, long number){
  int primes_found = 0; //only used for output formatting
  
  //iterate through primes field starting at the lowest primes
  for(int i = 0; i<primes_count; i++){

    //divide the input by current prime while it divides the input
    int prime_power = 0; //how many times did the prime divide the number
    while(number % primes[i] == 0){
      prime_power++;
      number /= primes[i];
    }

    //if the prime divides the input
    if(prime_power > 0){
      primes_found++;   
      print_prime(primes[i], prime_power, primes_found > 1);

      //if the 1 is all that's left from the input
      // -> all the primes forming the input have been found
      if(number == 1){
        printf("\n");
        break;
      }
    }
  }
  //if the number still isn't one it has larger factors than the precalculated primes
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

  long input;
  while(scanf("%ld", &input) == 1){ //main loop
    //end the program successfully if input is '0'
    if(input == 0){
      return 0;
    }
    //end the programm with 100 return code if input is negative
    if(input < 0){
      fprintf(stderr, "Error: Chybny vstup!\n");
      return 100;
    }
    //output special output for input '1' and end this iteration of while cycle
    if(input == 1){
      printf("Prvociselny rozklad cisla 1 je:\n1\n");
      continue;
    }

    //find & print the factors for every other input
    printf("Prvociselny rozklad cisla %ld je:\n", input);
    factorize_number(primes, primes_count, input);
    
  }

  //The program only reaches this when invalid input was scanned
  fprintf(stderr, "Error: Chybny vstup!\n");
  return 100;
}

