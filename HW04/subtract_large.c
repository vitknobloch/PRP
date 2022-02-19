#import <stdio.h>
#import <stdlib.h>

int str_len(char str[]){
    int len = 0;
    while(str[len] != '\0') len++;
    return len;
}


int sub_large_in_place(char str_large[],char str_small[]){   

    //save computing by saving the lenghts of strings
    int len_large = str_len(str_large);
    int len_small =  str_len(str_small);

    //initialize field for result
    char result_reverse[len_large];
    int rr_index = 0; //the next index in the result_reverse field to write to
    int last_not_null = 0; //the last index of digit other than '0'

    int carry = 0;

    //subtract digits starting from the end and carry if necessary
    for(int i = len_small - 1; i >=0; i--){ //this cycle only iterates when there are smaller number digits to go through
        char digit_diff = str_large[i + len_large - len_small] - str_small[i] - carry;
        if(digit_diff < 0){
            digit_diff = 10 + digit_diff;   //save the digit_diff + 10 to this digit 
            carry = 1;                      //and carry 1
        }
        else{
            carry = 0; //don't carry if digit diff is positive
        }
        //save digit_diff to result
        result_reverse[rr_index] = '0' + digit_diff;
        if(digit_diff != 0){
            last_not_null = rr_index;
        }
        rr_index++;
    }

    //iterate trough the rest of large number digits
    for(int i = len_large - len_small - 1; i >= 0; i--){
        if(carry == 1){ //if the carry is still one subtract one from the first not null digit in the large number
            if(str_large[i] > '0'){
                str_large[i] -= 1;
                carry = 0;
            }
            else{
                str_large[i] = '9';
            }
        }
        //save the digit to result
        result_reverse[rr_index] = str_large[i];
        if(str_large[i] != '0'){
            last_not_null = rr_index;
        }
        rr_index++;
    }

    //save the result to the input string
    int r_index = 0;
    //don't save the '0' digits
    for(rr_index = last_not_null; rr_index >= 0; rr_index--){
        str_large[r_index] = result_reverse[rr_index];
        r_index++;
    }
    str_large[r_index] = '\0';
    if(carry == 1){
        return -1;
    }
    if(last_not_null == 0){
        if(str_large[0] == '0'){
            return 1;
        }
    }
        
    return carry;
}

int main(int argc, char *argv[]){
    
    char str_large[101];
    char str_small[] = "2";

    scanf("%100s", str_large);

    int return_value = 0;
    long quotient = 1;
    while((return_value = sub_large_in_place(str_large, str_small))==0){
        quotient++;
    }

    if(return_value == -1){
        printf("%ld\n", quotient);
    }
    else{
        printf("result: %s\n", str_large);
        printf("%ld\n", quotient);
    }

    return 0;
}
