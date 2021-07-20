#include <stdio.h>
#include <GraphBLAS.h>

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


GrB_Info bfs(GrB_Vector result, GrB_Index source, GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph);

    GrB_Vector frontier, visited;
    GrB_Vector_new(&frontier, GrB_BOOL, nodes);
    GrB_Vector_new(&visited, GrB_UINT64, nodes);
    GrB_Vector_setElement_BOOL(frontier, true, source);

    GrB_Vector levels;
    GrB_Vector_new(&levels, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(levels, 0, source);

    //Descriptor set up
    GrB_Descriptor desc;
    GrB_Descriptor_new(&desc);
    GrB_Descriptor_set(desc, GrB_OUTP, GrB_REPLACE);
    GrB_Descriptor_set(desc, GrB_INP0, GrB_TRAN);
    GrB_Descriptor_set(desc, GrB_MASK, GrB_SCMP);


    //Constant setup for BFS
    int64_t level = 0;
    GrB_Index nvals = 0;

    //Do BFS, traverse graph

    while (nvals < nodes) {
        ++level;
        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL, GrB_LOR, visited, frontier, GrB_NULL);
        //print_vector_UINT64(visited, "Visited");
        GrB_mxv(frontier, visited, GrB_NULL, GxB_LOR_LAND_BOOL, graph, frontier, desc);
        GrB_Vector_assign_UINT64(levels, frontier, GrB_NULL, level, GrB_ALL, nodes, GrB_NULL);
        //print_vector_UINT64(frontier, "Wavefront");
        GrB_Vector_nvals(&nvals, visited);
    }

    GrB_Vector_dup(result, levels);

    GrB_Vector_free(&frontier);
    GrB_Vector_free(&visited);
    GrB_Descriptor_free(&desc);
    GrB_Vector_free(&levels);

    return GrB_SUCCESS;
}

GrB_Info sssp(GrB_Vector *result, GrB_Index source, GrB_Matrix graph) {
    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph);

    GrB_Vector dist, visited, frontier;
    GrB_Vector_new(&dist, GrB_UINT64, nodes);
    GrB_Vector_new(&visited, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(dist, 1, source);
    GrB_Vector_new(&frontier, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(frontier, true, source);

    //Descriptor set up
    GrB_Descriptor desc;
    GrB_Descriptor_new(&desc);
    GrB_Descriptor_set(desc, GrB_OUTP, GrB_REPLACE);
    GrB_Descriptor_set(desc, GrB_INP0, GrB_TRAN);
    GrB_Descriptor_set(desc, GrB_MASK, GrB_SCMP);


    //Constant setup for SSSP
    GrB_Index nvals = 0;

    //Traverse graph
    while (nvals < nodes) {
        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL, GrB_LOR, visited, frontier, GrB_NULL);
        //print_vector_UINT64(visited, "Visited");
        GrB_mxv(frontier, visited, GrB_NULL, GrB_MIN_PLUS_SEMIRING_UINT64, graph, frontier, desc);
        print_vector_UINT64(frontier, "distance");
        GrB_Vector_eWiseAdd_BinaryOp(dist, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, dist, frontier, GrB_NULL);
        print_vector_UINT64(dist, "distances");
        GrB_Vector_nvals(&nvals, visited);
    }

    GrB_Vector_dup(&result, frontier);

    //print_vector_UINT64(levels, "levels");
    //print_vector_UINT64(result, "res");

    GrB_Vector_free(&dist);
    GrB_Vector_free(&visited);
    GrB_Vector_free(&frontier);
    GrB_Descriptor_free(&desc);


    return GrB_SUCCESS;

}

int main(int argc, char **argv) {

    GrB_init(GrB_BLOCKING);

    //Adj matrix set up
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

    GrB_Vector result;
    GrB_Vector_new(&result, GrB_UINT64, NUM_NODES);

    print_vector_UINT64(result, "res, new");
    bfs(&result, SRC_NODE, graph);
    print_vector_UINT64(result, "res, final");

    //Cleanup
    GrB_Matrix_free(&graph);
    GrB_Vector_free(&result);

    GrB_finalize();
}