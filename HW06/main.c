#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_MATRICES 100

/*----------CUSTOM TYPES-----------*/
/*Type that stores info about one matrix*/
typedef struct
{
  int rows;
  int cols;
  int *values;
} Matrix;

/*----------FUNCTION DECLARATIONS + descriptions---------*/

/*Calculates the resulting matrix of a sequence and returns it*/
Matrix calculate_sequence(Matrix *matrices, char operators[], int m_count);

/*Checks if the next operator is '*' and calculates the multiplication
if it is, skips the processed matrices and returns the next index to look at*/
int check_for_multiplication(Matrix *matrices, char operators[], int current_index);

/*Loads sequence of Matrices and operators from stdin and returns their count*/
int input_matrices(Matrix *matrices, char operators[]);

/*Prints matrix to stdout*/
void print_matrix(Matrix m);

/*Replaces a matrix with another without memory leaks*/
void replace_matrix(Matrix *m_old, const Matrix m_new);

/*Returns a matrix that is result of addition of matrices in argument*/
Matrix matrix_sum(Matrix m1, Matrix m2);

/*Returns a matrix that is result of multiplication of matrices in argument*/
Matrix matrix_mul(Matrix m1, Matrix m2);

/*Returns a matrix that is result of subtraction of matrices in argument*/
Matrix matrix_sub(Matrix m1, Matrix m2);

/*Deallocate value fields of all stored matrices*/
void clear_memory(Matrix *matrices, int m_count);

/*Returns new matrix filled with values from arguments*/
Matrix create_matrix(int rows, int cols, int *values);

/*Loads new matrix from stdin and returns it*/
Matrix input_matrix();

/*Allocates memory of the given size*/
void* allocate_memory(int size);

/*Deallocates memory at the given address*/
void deallocate_memory(void **ptr);

/*----------IMPLEMENTATION----------*/
/* The main program */
int main(int argc, char *argv[])
{
  //field for inputed matrices
  Matrix matrices[MAX_INPUT_MATRICES];
  //field for inputed operators
  char operators[MAX_INPUT_MATRICES];

  //load matrices and operators to the maximal count
  int m_count = input_matrices(matrices, operators);
  if(m_count == -1){
    fprintf(stderr, "Error: Chybny vstup!\n");
    return 100;
  }

  //calculate result matrix and check for error during calculation
  Matrix result = calculate_sequence(matrices, operators, m_count);
  if(result.rows < 1 || result.cols < 1){
    fprintf(stderr, "Error: Chybny vstup!\n");
    clear_memory(matrices, m_count);
    return 100;
  }

  //output result and deallocate memory
  print_matrix(result);
  clear_memory(matrices, m_count);
  return 0;
}

/*Calculates the resulting matrix of a sequence and returns it*/
Matrix calculate_sequence(
  Matrix *matrices, char operators[], int m_count){
  int i = 0;//index of the next matrix and operation to look at
  while(i < m_count){
    Matrix m1 = matrices[i]; //keeps the first matrix of the next step
    char op = operators[i]; //keeps the operation of the next step
    if(op == '\0'){ //input function puts '\0' as operator after the last matrix
      return m1;
    }

    //this method always increments the 'i' index by at least 1
    //it also calculates all multiplications prior to current operations
    i = check_for_multiplication(matrices, operators, i);

    Matrix m2 = matrices[i]; //keeps the second matrix for the next step
    Matrix m; //result matrix
    
    //calculate operation
    switch (op){
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
        return create_matrix(-1, -1, NULL);
        break;
    }

    if(m.cols > 0 && m.rows > 0){
      //replace the second matrix with result if there was no error
      replace_matrix(&matrices[i], m);
    }
    else{
      //error output
      return create_matrix(-1, -1, NULL);
    }
  }
  return matrices[i];
}

/*Checks if the next operator is '*' and calculates the multiplication
if it is, skips the processed matrices and returns the next index to look at*/
int check_for_multiplication(
  Matrix *matrices, char operators[], int current_index){
  if(operators[current_index + 1]  == '*'){
    //save the first matrix of the multiplication
    Matrix m1 = matrices[current_index + 1];

    //recursively call this method to detect multiple multiplications
    int next_index = check_for_multiplication(matrices, operators, current_index+1);

    //save the second matrix of multiplications and calculate it
    Matrix m2 = matrices[next_index];
    Matrix m = matrix_mul(m1, m2);

    //save the result to the second matrix and return it's index
    replace_matrix(&matrices[next_index], m);
    return next_index;

  }else{
    //if the next operator isn't '*' -> look at the very next matrix
    return current_index + 1;
  }  
}

/*Loads sequence of Matrices and operators from stdin and returns their count*/
int input_matrices(Matrix *matrices, char operators[]){
  int m_count;
  for(m_count = 0; m_count < MAX_INPUT_MATRICES; m_count++){
    //input matrix and check for input error
    matrices[m_count] = input_matrix();
    if(matrices[m_count].rows == -1){
      clear_memory(matrices, m_count+1);
      return -1;
    }else if(matrices[m_count].rows == -2){
      m_count++;
      break;
    }

    //if it was the last matrix add terminating operator
    if(m_count == MAX_INPUT_MATRICES -1 || matrices[m_count].rows == -2){
      operators[m_count] = '\0';
      m_count++;
      break;
    }
    
    //input operator and check for input error
    char op;
    int read = scanf(" %c", &op);
    if(read == -1){
      operators[m_count] = '\0';
      m_count++;
      break;
    }else if(read != 1 || (op != '+' && op != '-' && op != '*')){      
      clear_memory(matrices, m_count+1);
      return -1;
    }
    operators[m_count] = op;
  }
  return m_count;
}

/*Prints matrix to stdout*/
void print_matrix(Matrix m){
  //header
  printf("%d %d\n", m.rows, m.cols);
  //values
  for(int i = 0; i < m.rows * m.cols; i++){
    printf("%d", m.values[i]);
    if(i % m.cols == m.cols -1){
      printf("\n");
    }else{
      printf(" ");
    }
  }

}

/*Replaces a matrix with another without memory leaks*/
void replace_matrix(Matrix *m_old, const Matrix m_new){
  deallocate_memory((void**)&(*m_old).values); //deallocate values of old matrix
  //replace properties with new ones
  (*m_old).rows = m_new.rows;
  (*m_old).cols = m_new.cols;
  (*m_old).values = m_new.values;
}

/*Returns a matrix that is result of addition of matrices in argument*/
Matrix matrix_sum(Matrix m1, Matrix m2){
  //check matrix sizes
  if(m1.cols != m2.cols || m1.rows != m2.rows){
    return create_matrix(-1, -1, NULL);
  }
  //sum each item
  int *values = allocate_memory(sizeof(int) * m1.rows * m1.cols);
  for(int i = 0; i < m1.rows * m1.cols; i++){
    values[i] = m1.values[i] + m2.values[i];
  }
  //return new matrix
  return create_matrix(m1.rows, m1.cols, values);
}

/*Returns a matrix that is result of subtraction of matrices in argument*/
Matrix matrix_sub(Matrix m1, Matrix m2){
  //check matrix sizes
  if(m1.cols != m2.cols || m1.rows != m2.rows){
    return create_matrix(-1, -1, NULL);
  }
  //subtract every item
  int *values = allocate_memory(sizeof(int) * m1.rows * m1.cols);
  for(int i = 0; i < m1.rows * m1.cols; i++){
    values[i] = m1.values[i] - m2.values[i];
  }
  //return new matrix
  return create_matrix(m1.rows, m1.cols, values);
}

/*Returns a matrix that is result of multiplication of matrices in argument*/
Matrix matrix_mul(Matrix m1, Matrix m2){
  //check matrix sizes
  if(m1.cols != m2.rows){
    return create_matrix(-1, -1, NULL);
  }
  //allocate memory for result values
  int *values = allocate_memory(sizeof(int) * m1.rows * m2.cols);
  //for every item of result matrix
  for(int r = 0; r < m1.rows; r++){
    for(int c = 0; c < m2.cols; c++){
      //calculate item result
      int s = 0;
      for(int i = 0; i < m1.cols; i++){
        s += m1.values[r*m1.cols + i] * m2.values[i*m2.cols + c];
      }
      values[r*m2.cols + c] = s;
    }
  }
  //return new matrix
  return create_matrix(m1.rows, m2.cols, values);
}

/*Deallocate value fields of all stored matrices*/
void clear_memory(Matrix *matrices, int m_count){
  for(int i = 0; i < m_count; i++){
    deallocate_memory((void**)&matrices[i].values);
  }
}

/*Returns new matrix filled with values from arguments*/
Matrix create_matrix(int rows, int cols, int *values){
  Matrix m;
  m.rows = rows;
  m.cols = cols;
  m.values = values;
  return m;
}

/*Loads new matrix from stdin and returns it*/
Matrix input_matrix(){
  //load matrix size and check for error
  int rows, cols;
  int read = scanf("%d %d", &rows, &cols);  
  if(read == -1){
    return create_matrix(-2, -2, NULL);
  }else if(read != 2){   
    return create_matrix(-1, -1, NULL);
  }else if(rows < 1 || cols < 1){
    return create_matrix(-1, -1, NULL);
  }

  //allocate memory for input values
  int *values = allocate_memory(sizeof(int) * rows * cols);

  //input values and check for errors
  for(int i = 0; i < rows * cols; i++){
    if(scanf("%d", &values[i]) != 1){      
      return create_matrix(-1, -1, values);
    }
  }

  //return new matrix
  return create_matrix(rows, cols, values);
}

/*Allocates memory of the given size*/
void* allocate_memory(int size){
  void * ptr = malloc(size);
  if(ptr == NULL){
    fprintf(stderr, "Error allocating memory!\n");
    exit(-1);
  }
  return ptr;
}

/*Deallocates memory at the given address*/
void deallocate_memory(void **ptr){  
  if(*ptr != NULL){
    free(*ptr);
    *ptr = NULL;
  }
}

