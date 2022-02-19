#include <stdio.h>
#include <stdlib.h>

#define INIT_LINE_SIZE 80

int search_file(FILE *file, const char *pattern, const int pattern_len,
                _Bool color, _Bool regex);
void *allocate_memory(int size);
void deallocate_memory(void **ptr);
void reallocate_memory(void **ptr, int size);
char *search_line(FILE *file, const char *pattern, const int pattern_len, 
                  _Bool color, _Bool regex);
int len(char const *str);
int color_sequence(char **str, int *line_size, int index_first, int len);
int str_is_equal(const char *str1, const char *str2);
int analyze_arguments(int argc, char *argv[], _Bool *color, _Bool *regex, 
                      char **pattern, FILE **file);
int match_pattern(const char *line, const char *pattern, int curr_index,
                  int match_index, int *match_len, _Bool regex);

/* The main program */
int main(int argc, char *argv[])
{  
  FILE *file = NULL;
  char *pattern;
  _Bool color = 0;
  _Bool regex = 0;
  
  //load arguments and handle errors
  int arg_eval = analyze_arguments(argc, argv, &color, &regex, &pattern, &file);

  if(arg_eval == 1){
    fprintf(stderr, "Missing required argument PATTERN\n");
    return 3;
  }
  int pattern_len = len(pattern);

  if (file == NULL){
    fprintf(stderr, "File not found or couldn't be read.\n");
    return 2;
  }

  //search file and return 1 if no match was found
  if(!search_file(file, pattern, pattern_len, color, regex)){
    fclose(file);
    return 1;
  }

  fclose(file);
  return 0;
}

/* 
 * Processes program args and saves them to passed adresses
 * returns 0 if arguments are valid and 1 if they are not
 */
int analyze_arguments(int argc, char *argv[], _Bool *color, _Bool *regex, 
                      char **pattern, FILE **file){
  _Bool pattern_set = 0;
  _Bool file_set = 0;
  //iterate through args
  for(int i = 1; i < argc; i++){
    if (str_is_equal("-E", argv[i]))
    {
      *regex = 1;
    }
    else if (str_is_equal("--color=always", argv[i]))
    {
      *color = 1;
    }
    else if (!pattern_set)
    {
      *pattern = argv[i];
      pattern_set = 1;
    }
    else
    {
      *file = fopen(argv[i], "r");
      file_set = 1;
    }
  }

  //if pattern wasn't set return 1
  if(!pattern_set){
    return 1;
  }
  //if file wasn't set set it to stdin
  if(!file_set){
    *file = stdin;
  }

  return 0;
}

/*Compares two strings, returns 1 if they match and 0 if they don't*/
int str_is_equal(const char *str1, const char *str2){
  int i = 0;
  while(str1[i] != '\0'){
    if(str1[i] != str2[i]){
      return 0;
    }
    i++;
  }

  if(str1[i] == str2[i]){
    return 1;
  }
  return 0;
}

/* 
 * Searches for pattern in file and prints matching lines
 * Returns number of matching lines
 */
int search_file(FILE *file, const char *pattern, const int pattern_len, 
                _Bool color, _Bool regex){

  int match_count = 0;
  char *line_pointer = search_line(file, pattern, pattern_len, color, regex);
  while(line_pointer != NULL){
    if(line_pointer[0] != '\0'){
      match_count++;
      printf("%s\n", line_pointer);
    }
    deallocate_memory((void**)&line_pointer);
    line_pointer = search_line(file, pattern, pattern_len, color, regex);
  }
  return match_count;
}

/*Returns line with found pattern (colored or not) or an empty string*/
char *search_line(FILE *file, const char *pattern, const int pattern_len, 
                  _Bool color, _Bool regex){
  int line_size = INIT_LINE_SIZE;
  char *line = allocate_memory(sizeof(char) * line_size);
  int match_index = 0;
  int match_len = 0;
  _Bool matched = 0;

  int c_int;
  for(int i = 0; i < line_size; i++){
    c_int = fgetc(file);
    //handle EOF and end of line
    if(c_int == EOF){
      if(i == 0){
        deallocate_memory((void**)&line);
        return NULL;
      }
      c_int = (int)'\0';
      
    }else if((char)c_int == '\n'){
      c_int = (int)'\0';
    }

    //add char to line
    if(i == line_size - 1){
      line_size *= 2;
      reallocate_memory((void**)&line, sizeof(char) * line_size);
    }
    line[i] = (char)c_int;

    //look for match
    match_index = match_pattern(line, pattern, i, match_index, &match_len, regex);

    //if matched
    if(match_index == -1){
      matched = 1;
      if (color){ //color match if found
        i = color_sequence(&line, &line_size, i - match_len, match_len);
        line[i] = (char)c_int;
      }
      match_index = 0;
      match_len = 0;
    }

    //break at the end of line or the end of file
    if((char)c_int == '\0'){
      break;
    }
  }
  //if no match was found return empty string
  if(!matched){
    line[0] = '\0';
  }
  return line;
}

/*Returns lenght of passed string*/
int len( char const *str){
  int counter = 0;
  while(str[counter] != '\0') counter++;
  return counter;
}

/*searches matches in lines, returns the next index in pattern to be matched*/
int match_pattern(const char *line, const char *pattern, int curr_index, 
                int match_index, int *match_len, _Bool regex){

  //return -1 to indicate match if the end of pattern was reached
  if (pattern[match_index] == '\0'){
    return -1;
  }

  //without regex
  if(!regex){
    if(line[curr_index] == pattern[match_index]){
      (*match_len)++;
      return match_index +1;
    }
    else{
      *match_len = 0;
      return 0;
    }
  }

  //with regex

  //if current char matches
  if(line[curr_index] == pattern[match_index]){
    (*match_len)++;
    //check for regex on next char
    if(pattern[match_index+1] == '?'){
      return match_index + 2;
    }
    if(pattern[match_index+1] == '*'){
      return match_index;
    }
    
    if(pattern[match_index+1] == '+'){
      return match_index +1;
    }
    
    return match_index +1;
  }

  //current char doesn't match
  //handle +
  if(pattern[match_index] == '+'){
    if(match_index < 1){
      *match_len = 0;
      return 0;
    }
    if(line[curr_index] == pattern[match_index -1]){
      (*match_len)++;
      return match_index;
    }
    return match_pattern(line, pattern, curr_index, match_index + 1, match_len, regex);
  }
  //handle ?
  if(pattern[match_index+1] == '?'){
    return match_pattern(line, pattern, curr_index, match_index + 2, match_len, regex);
  }
  //handle *
  if (pattern[match_index + 1] == '*')  {
    return match_pattern(line, pattern, curr_index, match_index + 2, match_len, regex);
  }

  //if no regex saved the match return 0 (not matched)
  *match_len = 0;
  return 0;
}

/*Colors matching sequence, returns index of the last character of sequence*/
int color_sequence(char **str, int *line_size, int index_first, int len){
  //reallocate memory for line if necessary
  if(index_first + len + 18 >= *line_size){
    *line_size *= 2;
    reallocate_memory((void **)str, sizeof(char) * (*line_size));
  }

  //save matching sequence to separate string
  char sequence[len];
  for(int i = 0; i < len; i++){    
    sequence[i] = (*str)[index_first + i];
  }

  const char esc_before[11] = {0x1b, 0x5b, 0x30, 0x31, 0x3b, 0x33,
                                                  0x31, 0x6d, 0x1b, 0x5b, 0x4b};
  const char esc_after[6] = {0x1b, 0x5b, 0x6d, 0x1b, 0x5b, 0x4b};
  //put escape sequence to color red to the line
  for(int i = 0; i < 11; i++){
    (*str)[index_first + i] = esc_before[i];
  }

  //put matched sequence to the line
  for(int i = 0; i < len; i++){
    (*str)[index_first + 11 + i] = sequence[i];
  }

  //put end of red escape sequence to line
  for (int i = 0; i < 6; i++){
    (*str)[index_first + 11 + len + i] = esc_after[i];
  }

  //return the index of last character of sequence
  return index_first + len + 17;  
}

/*Allocates memory of the given size*/
void *allocate_memory(int size)
{
  void *ptr = malloc(size);
  if (ptr == NULL)
  {
    fprintf(stderr, "Error allocating memory!\n");
    exit(-1);
  }
  return ptr;
}

/*Deallocates memory at the given address*/
void deallocate_memory(void **ptr)
{
  if (*ptr != NULL)
  {
    free(*ptr);
    *ptr = NULL;
  }
}

/*Reallocates memory to a block of the new size*/
void reallocate_memory(void **ptr, int size)
{
  *ptr = realloc(*ptr, size);
  if (*ptr == NULL)
  {
    fprintf(stderr, "Failed to re-allocate memory");
    exit(-2);
  }
}
