#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_MATRICES 26
#define INIT_SEQUENCE_LEN 20
#define INIT_MATRIX_VALUES_LEN 100

/* --------------FUNCTION DECLARATIONS + descriptions--------------*/

/*Calculates the resulting matrix of a sequence and returns it*/
struct Matrix calculate_sequence(struct Matrix *matrices, char *m_sequence,
                                 char *operators, int m_count, int seq_len);

/*Checks if the next operator is '*' and calculates the multiplication
if it is, it processes the multiplication sets the operator to '#' */
struct Matrix check_for_multiplication(
    struct Matrix *matrices, char *m_sequence, char *operators,
    int current_index, int m_count);

/*Returns new matrix with new memory block for 'values' field*/
struct Matrix copy_matrix(struct Matrix m);

/*Finds and returns Matrix from '*matrices' with the given name*/
struct Matrix find_by_name(char name, struct Matrix *matrices, int m_count);

/*Loads sequence of Matrices from stdin and returns their count*/
int input_matrices(struct Matrix *matrices);

/*Loads operator and checks for input error*/
char input_operator();

/*Prints matrix to stdout*/
void print_matrix(struct Matrix m);

/*Replaces a matrix with another without memory leaks*/
void replace_matrix(struct Matrix *m_old, const struct Matrix m_new);

/*Returns a matrix that is result of addition of matrices in argument*/
struct Matrix matrix_sum(struct Matrix m1, struct Matrix m2);

/*Returns a matrix that is result of multiplication of matrices in argument*/
struct Matrix matrix_mul(struct Matrix m1, struct Matrix m2);

/*Returns a matrix that is result of subtraction of matrices in argument*/
struct Matrix matrix_sub(struct Matrix m1, struct Matrix m2);

/*Deallocate value fields of all stored matrices*/
void clear_memory(struct Matrix *matrices, int m_count);

/*Returns new matrix filled with values from arguments*/
struct Matrix create_matrix(int rows, int cols, int *values, char name);

/*Loads new matrix from stdin and returns it*/
struct Matrix input_matrix();

/*Allocates memory of the given size*/
void *allocate_memory(int size);

/*Deallocates memory at the given address*/
void deallocate_memory(void **ptr);

/*Reallocates the memory and changes the pointer to new block of size 'size'*/
void reallocate_memory(void **ptr, int size);

/*Loads sequence of operations and matrices to execute*/
int input_sequence(char **m_names, char **operators);


/*----------CUSTOM TYPES-----------*/
/*Type that stores info about one matrix*/
struct Matrix
{
  char name;
  int rows;
  int cols;
  int *values;
};


/*----------IMPLEMENTATION----------*/
/* The main program */
int main(int argc, char *argv[])
{
  //field for inputed matrices
  struct Matrix matrices[MAX_INPUT_MATRICES];

  //load matrices and operators to the maximal count
  int m_count = input_matrices(matrices);
  if (m_count == -1)
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }

  //load sequence into '*m_sequence' and '*operators'
  char *m_sequence;
  char *operators;
  int seq_len = input_sequence(&m_sequence, &operators);
  if (seq_len < 0)
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
    clear_memory(matrices, m_count);
    return 100;
  }

  //calculate result matrix and check for error during calculation
  struct Matrix result =
      calculate_sequence(matrices, m_sequence, operators, m_count, seq_len);

  if (result.rows < 1 || result.cols < 1)
  {
    fprintf(stderr, "Error: Chybny vstup!\n");
    clear_memory(matrices, m_count);
    return 100;
  }

  //output result and deallocate memory
  print_matrix(result);
  deallocate_memory((void **)&result.values);
  deallocate_memory((void **)&m_sequence);
  deallocate_memory((void **)&operators);
  clear_memory(matrices, m_count);
  return 0;
}

/*Calculates the resulting matrix of a sequence and returns it*/
struct Matrix calculate_sequence(struct Matrix *matrices, char *m_sequence,
                                 char *operators, int m_count, int seq_len)
{
  //declare matrices m1, m2 to hold partial results
  struct Matrix m1 = copy_matrix(find_by_name(m_sequence[0], matrices, m_count));
  struct Matrix m2;
  
  //read instructions from sequence one by one
  for (int i = 0; i < seq_len; i++)
  {
    char op = operators[i];
    if (op == '\0')
    { //input function puts '\0' as operator after the last matrix
      return m1; //m1 holds the final results
    }
    /* '#' is special operator marking that the operation already was excuted 
    and should be skipped */
    if (op == '#')
    {
      continue;
    }

    /* this method returns next matrix in order or 
    result of multiplications with priority */
    m2 = check_for_multiplication(matrices, m_sequence, operators, i, m_count);
    if (m2.rows < 1)
    {
      deallocate_memory((void **)&m1.values);
      return create_matrix(-1, -1, NULL, '\0');
    }

    struct Matrix m; //result matrix

    //calculate operation
    switch (op)
    {
    case '+':
      m = matrix_sum(m1, m2);
      break;
    case '-':
      m = matrix_sub(m1, m2);
      break;
    case '*':
      m = matrix_mul(m1, m2);
      break;
    default:
      deallocate_memory((void **)&m1.values);
      deallocate_memory((void **)&m2.values);
      return create_matrix(-1, -1, NULL, '\0');
      break;
    }

    if (m.cols > 0 && m.rows > 0)
    {
      //replace the first matrix with result if there was no error
      replace_matrix(&m1, m);
      //clear the second matrix (will be allocated in next iteration)
      deallocate_memory((void **)&m2.values);
    }
    else
    {
      //error output
      deallocate_memory((void **)&m1.values);
      deallocate_memory((void **)&m2.values);
      return create_matrix(-1, -1, NULL, '\0');
    }
  }
  return m1;
}

/*Loads sequence of operations and matrices to execute*/
int input_sequence(char **m_names, char **operators)
{
  //allocate memory of initial size
  int input_size = INIT_SEQUENCE_LEN;
  *m_names = allocate_memory(sizeof(char) * input_size);
  *operators = allocate_memory(sizeof(char) * input_size);

  int index = 0;
  char op = ' ';
  while (op != '\0')
  {
    //reallocate if necessary
    if (index == input_size)
    {
      input_size *= 2;
      reallocate_memory((void **)m_names, sizeof(char) * input_size);
      reallocate_memory((void **)operators, sizeof(char) * input_size);
    }
    //input matrix name
    char c;
    if (scanf("%c", &c) != 1)
    {
      deallocate_memory((void **)m_names);
      deallocate_memory((void **)operators);
      return -1;
    }
    (*m_names)[index] = c;

    //input operator (last is '\0')
    op = input_operator();
    if (op == -1)
    {
      deallocate_memory((void **)m_names);
      deallocate_memory((void **)operators);
      return -1;
    }
    (*operators)[index] = op;

    index++;
  }
  return index; //returns lenght of sequence
}

/*Returns Matrix from '*matrices' with the given name*/
struct Matrix find_by_name(char name, struct Matrix *matrices, int m_count)
{
  for (int i = 0; i < m_count; i++)
  {
    if (matrices[i].name == name)
    {
      return matrices[i];
    }
  }
  return create_matrix(-1, -1, NULL, '\0');
}

/*Checks if the next operator is '*' and calculates the multiplication
if it is, it processes the multiplication sets the operator to '#' */
struct Matrix check_for_multiplication(
    struct Matrix *matrices, char *m_sequence, char *operators,
    int current_index, int m_count)
{

  if (operators[current_index + 1] == '*')
  {
    // '#' marks that operator has been executed and should be skipped
    operators[current_index + 1] = '#';

    //save copy of the first matrix for the multiplication
    struct Matrix m1 = copy_matrix(
        find_by_name(m_sequence[current_index + 1], matrices, m_count));

    //recursively call this method to detect multiple multiplications
    //save the result as second matrix of multiplication
    struct Matrix m2 = check_for_multiplication(matrices, m_sequence, operators,
                                                current_index + 1, m_count);

    //return result of multiplication
    struct Matrix m = matrix_mul(m1, m2);
    deallocate_memory((void **)&m1.values);
    deallocate_memory((void **)&m2.values);
    return m;
  }
  else
  {
    //if the next operator isn't '*' -> return copy of the next matrix
    return copy_matrix(
        find_by_name(m_sequence[current_index + 1], matrices, m_count));
  }
}

/*Returns new matrix with new block for values field*/
struct Matrix copy_matrix(struct Matrix m)
{
  //allocate new field of same size
  int *values = allocate_memory(sizeof(int) * m.rows * m.cols);
  //copy values
  for (int i = 0; i < m.rows * m.cols; i++)
  {
    values[i] = m.values[i];
  }
  //return new matrix
  return create_matrix(m.rows, m.cols, values, m.name);
}

/*Loads sequence of Matrices from stdin and returns their count*/
int input_matrices(struct Matrix *matrices)
{
  int m_count;
  for (m_count = 0; m_count < MAX_INPUT_MATRICES; m_count++)
  {
    //input matrix and check for input error
    struct Matrix m = input_matrix();
    if (m.rows == -1)
    {
      //clear memory in case of error
      clear_memory(matrices, m_count);
      return -1;
    }
    else if (m.rows == -2)
    {
      //break if file ended
      break;
    }
    matrices[m_count] = m;
  }
  return m_count;
}

/*Load operator and check for input error*/
char input_operator()
{
  //load from input
  char op;
  int read = scanf("%c", &op);
  if (read == -1)
  {
    return '\0'; //if file ended
  }
  else if (read != 1 || (op != '+' && op != '-' && op != '*'))
  {
    if (op == '\n')
    {
      return '\0'; //if line ended
    }
    return -1; //if 'op' is unexpected character
  }
  return op;
}

/*Prints matrix to stdout*/
void print_matrix(struct Matrix m)
{
  printf("[");
  //values
  for (int i = 0; i < m.rows * m.cols; i++)
  {
    printf("%d", m.values[i]);
    if (i % m.cols == m.cols - 1) //at the end of row
    {
      if (i == m.cols * m.rows - 1)
      {
        printf("]\n"); //if it's the last row
      }
      else
      {
        printf("; "); //if it's not the last row
      }
    }
    else
    {
      printf(" ");
    }
  }
}

/*Replaces a matrix with another without memory leaks*/
void replace_matrix(struct Matrix *m_old, const struct Matrix m_new)
{
  deallocate_memory((void **)&(*m_old).values); //deallocate values of old matrix
  //replace properties with new ones
  (*m_old).rows = m_new.rows;
  (*m_old).cols = m_new.cols;
  (*m_old).values = m_new.values;
}

/*Returns a matrix that is result of addition of matrices in argument*/
struct Matrix matrix_sum(struct Matrix m1, struct Matrix m2)
{
  //check matrix sizes
  if (m1.cols != m2.cols || m1.rows != m2.rows)
  {
    return create_matrix(-1, -1, NULL, '\0');
  }
  //sum each item
  int *values = allocate_memory(sizeof(int) * m1.rows * m1.cols);
  for (int i = 0; i < m1.rows * m1.cols; i++)
  {
    values[i] = m1.values[i] + m2.values[i];
  }
  //return new matrix
  return create_matrix(m1.rows, m1.cols, values, '\0');
}

/*Returns a matrix that is result of subtraction of matrices in argument*/
struct Matrix matrix_sub(struct Matrix m1, struct Matrix m2)
{
  //check matrix sizes
  if (m1.cols != m2.cols || m1.rows != m2.rows)
  {
    return create_matrix(-1, -1, NULL, '\0');
  }
  //subtract every item
  int *values = allocate_memory(sizeof(int) * m1.rows * m1.cols);
  for (int i = 0; i < m1.rows * m1.cols; i++)
  {
    values[i] = m1.values[i] - m2.values[i];
  }
  //return new matrix
  return create_matrix(m1.rows, m1.cols, values, '\0');
}

/*Returns a matrix that is result of multiplication of matrices in argument*/
struct Matrix matrix_mul(struct Matrix m1, struct Matrix m2)
{
  //check matrix sizes
  if (m1.cols != m2.rows)
  {
    return create_matrix(-1, -1, NULL, '\0');
  }
  //allocate memory for result values
  int *values = allocate_memory(sizeof(int) * m1.rows * m2.cols);
  //for every item of result matrix
  for (int r = 0; r < m1.rows; r++)
  {
    for (int c = 0; c < m2.cols; c++)
    {
      //calculate item result
      int s = 0;
      for (int i = 0; i < m1.cols; i++)
      {
        s += m1.values[r * m1.cols + i] * m2.values[i * m2.cols + c];
      }
      values[r * m2.cols + c] = s;
    }
  }
  //return new matrix
  return create_matrix(m1.rows, m2.cols, values, '\0');
}

/*Deallocate value fields of all stored matrices*/
void clear_memory(struct Matrix *matrices, int m_count)
{
  for (int i = 0; i < m_count; i++)
  {
    deallocate_memory((void **)&matrices[i].values);
  }
}

/*Returns new matrix filled with values from arguments*/
struct Matrix create_matrix(int rows, int cols, int *values, char name)
{
  struct Matrix m;
  m.name = name;
  m.rows = rows;
  m.cols = cols;
  m.values = values;
  return m;
}

/*Loads new matrix from stdin and returns it*/
struct Matrix input_matrix()
{
  //scan name char and check for error
  char name;
  if (scanf("%c", &name) != 1)
  {
    return create_matrix(-1, -1, NULL, '\0');
  }
  if (name == '\n') //end of matrix declaration part of input
  {
    return create_matrix(-2, -2, NULL, '\0');
  }

  //skip next 2 characters
  if (scanf("%*c%*c") != 0)
  {
    return create_matrix(-1, -1, NULL, '\0');
  }

  //allocate memory for input values
  int values_size = INIT_MATRIX_VALUES_LEN;
  int *values = allocate_memory(sizeof(int) * values_size);
  //count values and rows
  int values_count = 0;
  int rows = 0;

  char c = '[';
  //load all rows
  while (c != ']')
  {
    int n;
    //load row
    while (scanf("%d", &n) == 1){
      //if input field isn't large enough, reallocate it
      if (values_count == values_size)
      {
        values_size *= 2;
        reallocate_memory((void **)&values, sizeof(int) * values_size);
      }
      //save number to values, increment count
      values[values_count] = n;
      values_count++;
    }
    //number scan failed (row ended)
    rows++;
    //if scan failed or contains unexpected character -> error output
    if (scanf("%c", &c) != 1 || (c != ';' && c != ']'))
    {
      deallocate_memory((void **)&values);
      return create_matrix(-1, -1, NULL, '\0');
    };
  }
  if (scanf("%c", &c) != 1)
  { //skip '\n'
    deallocate_memory((void **)&values);
    return create_matrix(-1, -1, NULL, '\0');
  }

  //shrink the values field and return the new matrix
  reallocate_memory((void **)&values, sizeof(int) * values_count);
  return create_matrix(rows, values_count / rows, values, name);
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

/*Reallocates the memory and changes the pointer to new block of size 'size'*/
void reallocate_memory(void **ptr, int size)
{
  if (*ptr != NULL)
  {
    *ptr = realloc(*ptr, size);
    if (*ptr == NULL)
    {
      fprintf(stderr, "Error allocating memory!\n");
      exit(-1);
    }
  }
}
