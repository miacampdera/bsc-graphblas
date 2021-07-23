#include <stdio.h>
#include <GraphBLAS.h>
#include "uw_sssp.h"
#include "w_sssp.h"
#include "batch_sssp.h"

void print_matrix_UINT64(GrB_Matrix mat, char const *label)
{
    GrB_Index M,N;
    GrB_Matrix_nrows(&M, mat);
    GrB_Matrix_ncols(&N, mat);

    GrB_Index val;
    GrB_Info ret_val;

    printf("Matrix: %s =\n", label);
    for (GrB_Index row = 0; row < M; ++row) {
        printf("[");
        for (GrB_Index col = 0; col < N; ++col) {
            ret_val = GrB_Matrix_extractElement_UINT64(&val, mat, row, col);
            if (GrB_SUCCESS == ret_val) {
                if (col == 0) {
                    printf("%3ld", (long)val);
                } else {
                    printf(", %3ld", (long)val);
                }
            } else if (GrB_NO_VALUE == ret_val) {
                if (col == 0) {
                    printf("  -");
                } else {
                    printf(",   -");
                }
            } else {
                if (col == 0) {
                    printf("  ERR");
                } else {
                    printf(", ERR");
                }
            }
        }
        printf("]\n");
    }
}


void print_vector_UINT64(GrB_Vector vec, char const *label)
{
    GrB_Index N;
    GrB_Vector_size(&N, vec);

    GrB_Index val;
    GrB_Info ret_val;

    printf("Vector: %s =\n", label);

    printf("[");
    for (GrB_Index idx = 0; idx < N; ++idx) {
        ret_val = GrB_Vector_extractElement_UINT64(&val, vec, idx);
        if (GrB_SUCCESS == ret_val) {
            if (idx == 0) {
                printf("%3ld", (long)val);
            } else {
                printf(", %3ld", (long)val);
            }
        } else if (GrB_NO_VALUE == ret_val) {
            if (idx == 0) {
                printf("  -");
            } else {
                printf(",   -");
            }
        } else {
            if (idx == 0) {
                printf("  ERR");
            } else {
                printf(", ERR");
            }
        }
    }
    printf("]\n");
}

int main(int argc, char **argv) {

    //Initialize GraphBLAS enviroment
    GrB_init(GrB_BLOCKING);

    //Adjacency matrix set up
    GrB_Index const NUM_NODES = 7;
    GrB_Index const NUM_EDGES = 12;

    GrB_Index row_indices[] = {0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 6, 6};
    GrB_Index col_indices[] = {1, 3, 4, 6, 5, 0, 2, 5, 2, 2, 3, 4};

    bool values[] = {true, true, true, true, true, true, true, true, true, true, true, true};

    GrB_Matrix graph;
    GrB_Matrix_new(&graph, GrB_BOOL, NUM_NODES, NUM_NODES);
    GrB_Matrix_build_UINT64(graph, row_indices, col_indices, (bool*) values, NUM_EDGES, GrB_LOR);

    print_matrix_UINT64(graph, "Graph");

    //Source set up
    GrB_Index const SRC_NODE = 0;

    //Input, output vector set up
    GrB_Vector result;
    GrB_Vector_new(&result, GrB_UINT64, NUM_NODES);

    //Method call
    uw_sssp(&result, SRC_NODE, graph);

    //Print result
    print_vector_UINT64(result, "Node distances from source");

    //Free
    GrB_Matrix_free(&graph);
    GrB_Vector_free(&result);

    GrB_finalize();
}