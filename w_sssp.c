//
// Created by Mia Pulido Campdera on 20/6/21.
//
#include "w_sssp.h"

/* \brief Weighted single source shortest path algorithm. Calculates the shortest distance from a source node
 * to every other node in the graph.
 *
 * @param[in]   result  empty vector to fill in with calculated distances (graphblas vector)
 *              source  index of start node (graphblas index)
 *              graph   weighted adjacency matrix (graphblas matrix)
 *
 * @param[out]  result vector of distances, index of vector corresponds to node index.
 */
GrB_Info w_sssp(GrB_Vector result, GrB_Index source, GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph); //extract number of nodes from input matrix

    //Visited, distance and frontier vector declaration and initialization
    GrB_Vector distance, visited, frontier;
    GrB_Vector_new(&distance, GrB_UINT64, nodes);
    GrB_Vector_new(&visited, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(distance, 1, source); //set the start node to input source
    GrB_Vector_new(&frontier, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(frontier, true, source); //set the start node to input source

    //Descriptor set up
    GrB_Descriptor desc;
    GrB_Descriptor_new(&desc);
    GrB_Descriptor_set(desc, GrB_OUTP, GrB_REPLACE);
    GrB_Descriptor_set(desc, GrB_INP0, GrB_TRAN);
    GrB_Descriptor_set(desc, GrB_MASK, GrB_SCMP);


    //Constant set up
    GrB_Index nvals = 1000000;

    //Traverse graph
    while (nvals > 0) {
        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL, GrB_LOR, visited, frontier, GrB_NULL); //keep track of visited nodes
        GrB_mxv(frontier, visited, GrB_NULL, GrB_MIN_PLUS_SEMIRING_UINT64, graph, frontier, desc);
        GrB_Vector_eWiseAdd_BinaryOp(distance, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, distance, frontier, GrB_NULL);
        GrB_Vector_nvals(&nvals, frontier);
    }

    GrB_Vector_apply_BinaryOp1st_UINT64(distance, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, -1, distance, GrB_NULL);

    GrB_Vector_dup(result, distance); //copy final distance vector (levels) to result

    //Free
    GrB_Vector_free(&distance);
    GrB_Vector_free(&visited);
    GrB_Vector_free(&frontier);
    GrB_Descriptor_free(&desc);

    return GrB_SUCCESS;
}
