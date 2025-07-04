typedef struct Matrix
{
    int num_rows;
    int num_cols;
    long long int **data;
} Matrix;

Matrix *create_matrix(int r, int c);

void destroy_matrix(Matrix *m);

Matrix *add_matrix(Matrix *A, Matrix *B);

long long int f(Matrix *A, Matrix *B, int k, int m);

Matrix *mult_matrix(Matrix *A, Matrix *B);

Matrix *scalar_mult_matrix(int s, Matrix *M);

Matrix *transpose_matrix(Matrix *A);

long long int **mat(long long int **M, int rows, int cols, int k, int l);

long long int det(long long int **m, int rows, int cols);

long long int determinant(Matrix *M);

void print_matrix(Matrix *m);

Matrix *read_matrix_from_file(char *a);

void write_matrix_to_file(Matrix *m, char *a);