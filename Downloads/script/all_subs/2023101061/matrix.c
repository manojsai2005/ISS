#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Matrix
{
    int num_rows;
    int num_cols;
    long long int **data;
} Matrix;
Matrix *create_matrix(int r, int c)
{
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    m->num_rows = r;
    m->num_cols = c;
    m->data = (long long int **)calloc(r, sizeof(long long int *));
    for (int i = 0; i < r; i++)
    {
        m->data[i] = (long long int *)calloc(c, sizeof(long long int));
    }
    return m;
}
void destroy_matrix(Matrix *m)
{
    if (m == NULL)
    {
        return;
    }
    for (int i = 0; i < m->num_rows; i++)
    {
        free(m->data[i]);
    }
    free(m->data);
    free(m);
}
Matrix *add_matrix(Matrix *A, Matrix *B)
{
    Matrix* m = create_matrix(A->num_rows, A->num_cols);
    for (int i = 0; i < A->num_rows; i++)
    {
        for (int j = 0; j < A->num_cols; j++)
        {
            m->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }
    return m;
}
long long int f(Matrix *A, Matrix *B, int k, int m)
{
    long long int s = 0;
    for (int j = 0; j < (A->num_cols); j++)
    {
        s = s + (A->data[k][j]) * (B->data[j][m]);
    }
    return s;
}
Matrix *mult_matrix(Matrix *A, Matrix *B)
{
    Matrix *m = create_matrix(A->num_rows, B->num_cols);
    long long int s = 0;
    for (int i = 0; i < A->num_rows; i++)
    {
        for (int j = 0; j < B->num_cols; j++)
        {
            m->data[i][j] = f(A, B, i, j);
        }
    }
    return m;
}
Matrix *scalar_mult_matrix(int s, Matrix *M)
{
    Matrix *m = create_matrix(M->num_rows, M->num_cols);
    for (int i = 0; i < M->num_rows; i++)
    {
        for (int j = 0; j < M->num_cols; j++)
        {
            m->data[i][j] = s * (M->data[i][j]);
        }
    }
    return m;
}
Matrix *transpose_matrix(Matrix *A)
{
    Matrix *m = create_matrix(A->num_cols, A->num_rows);
    for (int i = 0; i < A->num_cols; i++)
    {
        for (int j = 0; j < A->num_rows; j++)
        {
            m->data[i][j] = (A->data[j][i]);
        }
    }
    return m;
}
long long int **mat(long long int **M, int rows, int cols, int k, int l)
{
    long long int **m = (long long int **)calloc(rows - 1, sizeof(long long int *));
    for (int i = 0; i < rows - 1; i++)
    {
        m[i] = (long long int *)calloc(cols - 1, sizeof(long long int));
    }
    int i_m = 0, j_m = 0;
    for (int i = 1; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (j == l)
            {
                continue;
            }
            m[i_m][j_m] = M[i][j];
            j_m++;
        }
        j_m = 0;
        i_m++;
    }
    return m;
}
long long int det(long long int **m, int rows, int cols)
{
    long long int s = 0;
    if (rows == 1 && cols == 1)
    {
        return m[0][0];
    }
    for (int j = 0; j < cols; j++)
    {
        if (((j) % 2) == 0)
            s = s + (m[0][j]) * (det(mat(m, rows, cols, 0, j), rows - 1, cols - 1));
        else
            s = s - (m[0][j]) * (det(mat(m, rows, cols, 0, j), rows - 1, cols - 1));
    }
    return s;
}
long long int determinant(Matrix *M)
{
    long long int d;
    if (M->num_cols != M->num_rows)
    {
        return -1;
    }
    else
    {
        d = det(M->data, M->num_rows, M->num_cols);
        return d;
    }
}
void print_matrix(Matrix *m)
{
    printf("%d %d\n", m->num_rows, m->num_cols);
    for (int i = 0; i < m->num_rows; i++)
    {
        for (int j = 0; j < m->num_cols; j++)
        {
            printf("%lld ", m->data[i][j]);
        }
        printf("\n");
    }
}
Matrix *read_matrix_from_file(char a[100])
{
    FILE *fptr;
    fptr = fopen(a, "r");
    if (fptr == NULL)
    {
        fclose(fptr);
        return NULL;
    }
    int rows, cols;
    fscanf(fptr, "%d %d", &rows, &cols);
    Matrix *m = create_matrix(rows, cols);
    int state=0;

    for (int i = 0; i < m->num_rows; i++)
    {
        for (int j = 0; j < m->num_cols; j++)
        {
            if (!feof(fptr))
            {
                fscanf(fptr, "%lld", &m->data[i][j]);
            }
            else
            {
                printf("INVALID");
                state = 1;
                break;
            }
            
        }
        if (state==1) break;
    }
    fclose(fptr);
    return m;
}
void write_matrix_to_file(Matrix *m, char *a)
{
    FILE *fptr;
    fptr = fopen(a, "w");
    if (fptr==NULL) return;
    fprintf(fptr, "%d %d\n", m->num_rows, m->num_cols);
    for (int i = 0; i < m->num_rows; i++)
    {
        for (int j = 0; j < m->num_cols; j++)
        {
            fprintf(fptr, "%lld ", m->data[i][j]);
        }
        fprintf(fptr, "\n");
    }
    fclose(fptr);
}