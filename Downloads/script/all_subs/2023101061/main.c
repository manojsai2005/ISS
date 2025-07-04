#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "matrix.h"
int main()
{
    int n;
    scanf("%d", &n);
    int k = 0;
    int o;
    for (int w = 0; w < n; w++)
    {
        char ar[20];
        scanf("%s", ar);
        FILE *his = fopen("mx_history", "a");
        if (strcmp(ar, "history") == 0)
        {
            fprintf(his, "LOG::%s", ar);
            fprintf(his, "\n");
            fclose(his);
            char info[4096];
            FILE *fptr = fopen("mx_history", "r");
            while (fgets(info, 4096, fptr) != NULL)
            {
                printf("%s", info);
            }
            fclose(fptr);
        }
        else
        {
            int type;
            scanf("%d", &type);
            if ((strcmp(ar, "add_matrix") == 0) && (type == 0))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                int a, b;
                scanf("%d %d", &a, &b);
                Matrix *A = create_matrix(a, b);
                for (int i = 0; i < a; i++)
                {
                    for (int j = 0; j < b; j++)
                    {
                        scanf("%lld", &A->data[i][j]);
                    }
                }
                int c, d;
                scanf("%d %d", &c, &d);
                Matrix *B = create_matrix(c, d);
                for (int i = 0; i < c; i++)
                {
                    for (int j = 0; j < d; j++)
                    {
                        scanf("%lld", &B->data[i][j]);
                    }
                }
                if (a == c && b == d)
                {
                    Matrix* m = add_matrix(A, B);
                    destroy_matrix(B);
                    destroy_matrix(A);
                    print_matrix(m);
                    destroy_matrix(m);
                }
                else
                {
                    printf("ERROR: INVALID ARGUMENT\n");
                }
            }
            else if ((strcmp(ar, "scalar_mult_matrix") == 0) && (type == 0))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                int a, b;
                int s;
                scanf("%d", &s);
                scanf("%d %d", &a, &b);
                Matrix *A = create_matrix(a, b);
                
                for (int i = 0; i < a; i++)
                {
                    for (int j = 0; j < b; j++)
                    {
                        scanf("%lld", &A->data[i][j]);
                    }
                }
                Matrix*mat = scalar_mult_matrix(s, A);
                print_matrix(mat);
                destroy_matrix(A);
                destroy_matrix(mat);
            }
            else if ((strcmp(ar, "mult_matrix") == 0) && (type == 0))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                int a, b;
                scanf("%d %d", &a, &b);
                Matrix *A = create_matrix(a, b);
                for (int i = 0; i < a; i++)
                {
                    for (int j = 0; j < b; j++)
                    {
                        scanf("%lld", &A->data[i][j]);
                    }
                }
                int c, d;
                scanf("%d %d", &c, &d);
                Matrix *B = create_matrix(c, d);
                for (int i = 0; i < c; i++)
                {
                    for (int j = 0; j < d; j++)
                    {
                        scanf("%lld", &B->data[i][j]);
                    }
                }
                Matrix *mat;
                if (b == c)
                {
                    mat = mult_matrix(A, B);
                    print_matrix(mat);
                }
                else
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                destroy_matrix(A);
                destroy_matrix(B);
                destroy_matrix(mat);
            }
            else if ((strcmp(ar, "transpose_matrix") == 0) && (type == 0))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                int a, b;
                scanf("%d %d", &a, &b);
                Matrix *A = create_matrix(a, b);
                for (int i = 0; i < a; i++)
                {
                    for (int j = 0; j < b; j++)
                    {
                        scanf("%lld", &A->data[i][j]);
                    }
                }
                Matrix *mat = transpose_matrix(A);
                print_matrix(mat);
                destroy_matrix(A);
                destroy_matrix(mat);
            }
            else if ((strcmp(ar, "determinant") == 0) && (type == 0))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                int a, b;
                long long int s;
                scanf("%d %d", &a, &b);
                Matrix *A = create_matrix(a, b);
                for (int i = 0; i < a; i++)
                {
                    for (int j = 0; j < b; j++)
                    {
                        scanf("%lld", &A->data[i][j]);
                    }
                }
                if (A->num_rows != A->num_cols)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                else
                {
                    s = determinant(A);
                    printf("%lld", s);
                }
                destroy_matrix(A);
            }
            else if ((strcmp(ar, "add_matrix") == 0) && (type == 1))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                char bb[100], aa[100], cc[100];
                scanf("%s", aa);
                scanf("%s", bb);
                scanf("%s", cc);

                Matrix *A;
                A = read_matrix_from_file(aa);
                Matrix *B;
                B = read_matrix_from_file(bb);
                if (B == NULL ||A == NULL)
                {
                    printf("ERROR: INVALID ARGUMENT");
                    continue;
                }
                Matrix *list;
                if (A->num_rows == B->num_rows && A->num_cols == B->num_cols)
                {
                    list = add_matrix(A, B);
                    write_matrix_to_file(list, cc);
                }
                else
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                destroy_matrix(A);
                destroy_matrix(B);
                destroy_matrix(list);
            }
            else if ((strcmp(ar, "scalar_mult_matrix") == 0) && (type == 1))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                char aa[100], bb[100];
                int o;
                scanf("%s", aa);
                scanf("%s", bb);
                scanf("%d", &o);
                Matrix *A;
                A = read_matrix_from_file(aa);
                if (A == NULL)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                Matrix *list = scalar_mult_matrix(o, A);
                write_matrix_to_file(list, bb);
                destroy_matrix(A);
                destroy_matrix(list);
            }
            else if ((strcmp(ar, "mult_matrix") == 0) && (type == 1))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                char bb[100], aa[100], cc[100];
                scanf("%s", aa);
                scanf("%s", bb);
                scanf("%s", cc);
                Matrix *A;
                A = read_matrix_from_file(aa);
                print_matrix(A);
                Matrix *B;
                B = read_matrix_from_file(bb);
                if (B == NULL||A == NULL)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                Matrix *list;
                if (A->num_cols == B->num_rows)
                {
                    list = mult_matrix(A, B);
                    write_matrix_to_file(list, cc);
                }
                else
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                destroy_matrix(A);
                destroy_matrix(B);
                destroy_matrix(list);
            }
            else if ((strcmp(ar, "transpose_matrix") == 0) && (type == 1))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                char aa[100], bb[100];
                scanf("%s", aa);
                scanf("%s", bb);
                Matrix *A;
                A = read_matrix_from_file(aa);
                if (A == NULL)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                Matrix *list = transpose_matrix(A);
                write_matrix_to_file(list, bb);
                destroy_matrix(A);
                destroy_matrix(list);
            }
            else if ((strcmp(ar, "determinant") == 0) && (type == 1))
            {
                fprintf(his, "LOG::%s %d", ar, type);
                fprintf(his, "\n");
                fclose(his);
                char aa[100];
                scanf("%s", aa);
                long long int s;
                Matrix *A;
                A = read_matrix_from_file(aa);
                if (A == NULL)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                if (A->num_rows != A->num_cols)
                {
                    printf("ERROR: INVALID ARGUMENT");
                }
                else
                {
                    s = determinant(A);
                    printf("%lld", s);
                }
                destroy_matrix(A);
            }
        }
    }
    return 0;
}