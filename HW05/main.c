#include <stdio.h>
#include <stdlib.h>

#define INIT_ALLOC_SIZE 20

void* allocate_memory(int size);
void deallocate_memory(void **ptr);
void reallocate_memory(void **ptr, int size);
int input_line(char **line_ptr);
char shift_char(char src, int offset);
void shift(const char *src, char *dst, int offset);
int str_diff(const char *str1, const char *str2);
int char_dist(char coded_char, char listened_char);
void solve_non_optional(char *code, char *listened, char *result, int code_len);
void solve_optional(char *code, char *listened, char * result, int code_len, 
                    int listened_len);
int str_diff_opt(char* row_str, int row_len, char* column_str, int column_len);
void debug_print_levenstein(char *row_str, char *column_str, int row_len, 
                            int column_len);

/* The main program */
int main(int argc, char *argv[])
{
  // read the two lines of input and store them
  char *code = NULL;
  int code_len = input_line(&code);
  char *listened = NULL;
  int listened_len = input_line(&listened);  

  //Free memory and return error code 100 for wrong char in input
  if(code_len == -1 || listened_len == -1){
    fprintf(stderr, "Error: Chybny vstup!\n");
    deallocate_memory((void**)&code);
    deallocate_memory((void**)&listened);
    return 100;
  }

  //init result field
  char *result = allocate_memory(sizeof(char) * code_len);

  if (argc > 1 && str_diff(argv[1], "-prp-optional") == 0){
    //optional assignment
    solve_optional(code, listened, result, code_len, listened_len);
  }
  else{
    //non-optional assignment
    if(code_len != listened_len){
      //Free memory and return error code 101 for different input lenght
      deallocate_memory((void**)&code);
      deallocate_memory((void**)&listened);
      deallocate_memory((void**)&result);
      fprintf(stderr, "Error: Chybna delka vstupu!\n");
      return 101;
    }
    solve_non_optional(code, listened, result, code_len);
  }

  //print result to stdout
  printf("%s\n", result);

  //free all allocated memory
  deallocate_memory((void**)&code);
  deallocate_memory((void**)&listened);
  deallocate_memory((void**)&result);
  
  return 0;
}

/*Solves the optional assignment and saves result to result*/
void solve_optional(char *code, char *listened, char *result, int code_len, int listened_len){
  int best_offset = 0;
  int best_strdiff = -1;
  //try every possible shift & save the best shift
  for(int i = 0; i < 52; i++){
    shift(code, result, i);
    //compare listened string and shifted string
    int strdiff = str_diff_opt(listened, listened_len, result, code_len);
    if (strdiff < best_strdiff || best_strdiff == -1){
      best_strdiff = strdiff;
      best_offset = i;
    }
  }
  //save the best shift to result
  shift(code, result, best_offset);
}

/*Calculates Levenstein distance between two strings*/
int str_diff_opt(char* row_str, int row_len, char* column_str, int column_len){
  //allocate memory for Wagner-Fisher algorithm
  int *wagner_fisher = allocate_memory(sizeof(int) * row_len * column_len);
  //init the table
  for(int i=0; i < row_len; i++){
    wagner_fisher[i] = i;
  }
  for(int i=0; i < column_len; i++){
    wagner_fisher[i * row_len] = i;
  }

  //run the algorithm
  for(int r = 1; r < row_len; r++){
    for(int c = 1; c < column_len; c++){
      int substitution_cost = row_str[r-1] == column_str[c-1] ? 0 : 1;
      int best = wagner_fisher[(c-1) * row_len + (r-1)] + substitution_cost;
      if(wagner_fisher[(c-1) * row_len + r] + 1 < best){
        best = wagner_fisher[(c-1) * row_len + r] + 1;
      }
      if(wagner_fisher[c * row_len + (r-1)] + 1 < best){
        best = wagner_fisher[c * row_len + (r-1)] + 1;
      }
      wagner_fisher[c * row_len + r] = best;
    }
  }
  //save the result
  int str_diff = wagner_fisher[column_len * row_len -1];
  //free memory
  deallocate_memory((void**)&wagner_fisher);
  return str_diff;
}

/*Solves non-optional assignment and saves result to result*/
void solve_non_optional(char *code, char *listened, char * result, int code_len){
  int best_offset = 0;
  int best_strdiff = -1;
  //try all possible shifts
  for(int i = 0; i < 52; i++){
    //try shift and calculate difference
    shift(code, result, i);
    int strdiff = str_diff(listened, result);
    //save the offset if it's better than previous
    if (strdiff < best_strdiff || best_strdiff == -1){
      best_strdiff = strdiff;
      best_offset = i;
      //if more than half the chars are right - no shift can be better
      if(strdiff < code_len / 2){
        break;
      }
    }
  }
  //save best shift to result
  shift(code, result, best_offset);
}

/*Compares individual chars of 2 strings and returns number of different chars*/
int str_diff(const char *str1, const char *str2){
  int result = 0;
  int i = 0;
  while(str1[i] != '\0' && str2[i] != '\0'){
    if(str1[i] != str2[i]){
      result++;
    }
    i++;
  }
  if(str1[i] != str2[i]){
    //return -1 for different string lenghts
    result = -1;
  }
  return result;
}

/*Shifts a string by given offset and saves result to dst*/
void shift(const char *src, char *dst, int offset){
  int i = 0;
  while(src[i] != '\0'){
    dst[i] = shift_char(src[i], offset);
    i++;
  }
  dst[i] = '\0';
}

/*Shifts one character by given offset*/
char shift_char(char src, int offset){
  //parse the chars to of value be 0-51
  if(src >= 'a'){
    src -= 'a' - 26;
  }
  else if (src >= 'A'){
    src -= 'A';
  }

  //shift the normalized char
  src = (src + offset) % 52;
  //found out that modulo in C can have negative result
  //found this code to fix online
  src = src < 0 ? src + 52 : src;

  //deparse the shifted char
  src += 'A';
  if(src > 'Z'){
    src += 'a' - 'Z' - 1;
  }

  return src;
}

/*Calculate the offset between two normalized chars*/
int char_dist(char coded_char, char listened_char){
  //parse the chars to be -51
  if(coded_char >= 'a'){
    coded_char -= 'a' - 26;
  }
  else if (coded_char >= 'A'){
    coded_char -= 'A';
  }

  if(listened_char >= 'a'){
    listened_char -= 'a' - 26;
  }
  else if (listened_char >= 'A'){
    listened_char -= 'A';
  }

  //return the distance of normalized chars
  return listened_char - coded_char;

}

/*Loads one line from input and saves string to the given parameter,
  - automatically alocates memory and returns lenght of string*/
int input_line(char **line_ptr){
  int alloc_size = INIT_ALLOC_SIZE;
  //alocate memory for INIT_ALLOC_SIZE chars including '\0'
  char *line = allocate_memory(sizeof(char) * alloc_size);
  *line_ptr = line;

  int i = 0;
  char c;
  //save chars to string until end of line
  do{
    //check that the input is correct
    if(scanf("%c", &c) != 1){
      return -1;
    }
    if((c < 'A' || (c > 'Z' &&  c < 'a') || c > 'z') && c != '\n'){
      return -1;
    }

    //if ran out of allocated memory reallocate
    if (i >= alloc_size){ 
      alloc_size *= 2;
      reallocate_memory((void**)&line, alloc_size);
      *line_ptr = line;
    }
    //save the loaded char to string
    line[i] = c;
    //end string at the end of line
    if(c == '\n'){
      line[i] = '\0';
    }
    i++;
  }while(c != '\n');
  return i;
}

/*Allocated memory of given size and returns address if that memory*/
void* allocate_memory(int size){
  void *ptr = malloc(size);
  if(ptr == NULL){
    fprintf(stderr, "Failed to allocate memory");
    exit(-1);
  }
  return ptr;
}

/*Dealloctes memory at the given address*/
void deallocate_memory(void **ptr){
  if(*ptr != NULL){
    free(*ptr);
  }
}

/*Reallocates memory to a block of the new size*/
void reallocate_memory(void **ptr, int size){
  *ptr = realloc(*ptr, size);
  if(*ptr == NULL){
    fprintf(stderr, "Failed to re-allocate memory");
    exit(-2);
  }
}

