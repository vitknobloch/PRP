#include <stdio.h>
#include <stdlib.h>

/*divides large number digit by digit as a human*/
int divide_large(char large_int[], int divisor, int num_len, _Bool rewrite_large){
    //initialize properties
    int divident = 0;
    int remainder = 0;
    char result[num_len];

    int not_null_count = 0; //used to not copy zeros in the begining to the result
    int next_index = 0;
    while(large_int[next_index] != -1){
        //write down the next digit to the remainer from previous step (just like people)
        divident = remainder * 10 + large_int[next_index];
        remainder = divident % divisor; //save the remainder of this step for use in next step

        if(rewrite_large){ //in order to save a lot of computing only save the result if told to
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

int str_len(char str[]){
    int len = 0;
    while (str[len] != '\0') len++;
    return len;
}

int num_len(char num[]){
    int len = 0;
    while (num[len] != -1)  len++;
    return len;
}

int parse_large_int(char str[]){
    int len = 0;
    while (str[len] != '\0') {
        str[len] -= '0';
        if(str[len] > 9 || str[len] < 0){
            fprintf(stderr, "Invalid input for large int");
            return -1;
        }
        len++;
    }
    str[len] = -1;
    return len;
}

int large_int_to_string(char str[]){
    int len = 0;
    while (str[len] != -1) {
        str[len] += '0';
        len++;
    }
    str[len] = '\0';
    return len;
}

int main(int argc, char *argv[]){

    char large_int[101];
    int divisor = 20;
    scanf("%100s", large_int);
    scanf("%d", &divisor);

    int num_len = parse_large_int(large_int);
    int remainder = divide_large(large_int, divisor, num_len, 1);
    
    printf("Remainder: %d\n", remainder);
    large_int_to_string(large_int);
    printf("Result:    %s\n", large_int);

    return 0;
}
