#include <stdio.h>
#include <GraphBLAS.h>
#include "uw_sssp.h"
#include "w_sssp.h"
#include "batch_sssp.h"

void print_matrix_UINT64(GrB_Matrix matrix, char const *label)
{
    GrB_Index rows, cols;
    GrB_Matrix_nrows(&rows, matrix);
    GrB_Matrix_ncols(&cols, matrix);

    GrB_Index val;
    GrB_Info ret_val;

    printf("Matrix: %s =\n", label);
    for (GrB_Index row = 0; row < rows; ++row) {
        printf("[");
        for (GrB_Index col = 0; col < cols; ++col) {
            ret_val = GrB_Matrix_extractElement_UINT64(&val, matrix, row, col);
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
                    printf("  error");
                } else {
                    printf(", error");
                }
            }
        }
        printf("]\n");
    }
}

void print_vector_UINT64(GrB_Vector vector, char const *label)
{
    GrB_Index index;
    GrB_Vector_size(&index, vector);

    GrB_Index val;
    GrB_Info ret_val;

    printf("Vector: %s =\n", label);

    printf("[");
    for (GrB_Index idx = 0; idx < index; ++idx) {
        ret_val = GrB_Vector_extractElement_UINT64(&val, vector, idx);
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
                printf("  error");
            } else {
                printf(", error");
            }
        }
    }
    printf("]\n");
}

void run_uw() {
    GrB_Index const NUM_NODES = 5;
    GrB_Index const NUM_EDGES = 9;

    GrB_Index row_indices[] = {0, 0, 1, 1, 2, 2, 3, 3, 4};
    GrB_Index col_indices[] = {1, 2, 2, 4, 3, 4, 0, 4, 1};

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
}

void run_w() {
    GrB_Index const NUM_NODES = 5;
    GrB_Index const NUM_EDGES = 9;

    GrB_Index row_indices[] = {0, 0, 1, 1, 2, 2, 3, 3, 4};
    GrB_Index col_indices[] = {1, 2, 2, 4, 3, 4, 0, 4, 1};

    int64_t values[] = {1, 4, 3, 2, 2, 1, 3, 4, 1};

    GrB_Matrix graph;
    GrB_Matrix_new(&graph, GrB_UINT64, NUM_NODES, NUM_NODES);
    GrB_Matrix_build_UINT64(graph, row_indices, col_indices, values, NUM_EDGES, GrB_PLUS_UINT64);

    print_matrix_UINT64(graph, "Graph");

    //Source set up
    GrB_Index const SRC_NODE = 0;

    //Input, output vector set up
    GrB_Vector result;
    GrB_Vector_new(&result, GrB_UINT64, NUM_NODES);

    //Method call
    w_sssp(&result, SRC_NODE, graph);

    //Print result
    print_vector_UINT64(result, "Node distances from source");

    //Free
    GrB_Matrix_free(&graph);
    GrB_Vector_free(&result);
}

void run_batch() {
    //Adjacency matrix set up
    GrB_Index const NUM_NODES = 5;
    GrB_Index const NUM_EDGES = 9;

    GrB_Index row_indices[] = {0, 0, 1, 1, 2, 2, 3, 3, 4};
    GrB_Index col_indices[] = {1, 2, 2, 4, 3, 4, 0, 4, 1};

    bool values[] = {true, true, true, true, true, true, true, true, true, true, true, true};

    GrB_Matrix graph;
    GrB_Matrix_new(&graph, GrB_BOOL, NUM_NODES, NUM_NODES);
    GrB_Matrix_build_UINT64(graph, row_indices, col_indices, (bool*) values, NUM_EDGES, GrB_LOR);

    print_matrix_UINT64(graph, "Graph");

    GrB_Matrix distances;
    GrB_Matrix_new(&distances, GrB_UINT64, NUM_NODES, NUM_NODES);

    batch_sssp(&distances, graph);

    print_matrix_UINT64(distances, "distances");

    //Free
    GrB_Matrix_free(&graph);
    GrB_Matrix_free(&distances);
}

int main(int argc, char **argv) {

    //Initialize GraphBLAS environment
    GrB_init(GrB_BLOCKING);

    run_w();

    //Finalize GraphBLAS environment
    GrB_finalize();
}
