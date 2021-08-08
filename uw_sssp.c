//
// Created by Mia Pulido Campdera on 12/6/21.
//
#include "uw_sssp.h"

/* \brief Unweighted single source shortest path algorithm. Calculates the shortest distance from a source node
 * to every other node in the graph.
 *
 * @param[in]   result  empty vector to fill in with calculated distances (graphblas vector)
 *              source  index of start node (graphblas index)
 *              graph   unweighted adjacency matrix (graphblas matrix)
 *
 * @param[out]  result vector of distances, index of vector corresponds to node index.
 */
GrB_Info uw_sssp(GrB_Vector result, GrB_Index source, GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph); //extract number of nodes from input matrix

    //Visited and frontier vector declaration and initialization
    GrB_Vector frontier, visited;
    GrB_Vector_new(&frontier, GrB_BOOL, nodes);
    GrB_Vector_new(&visited, GrB_UINT64, nodes);
    GrB_Vector_setElement_BOOL(frontier, true, source); //set the start node to input source

    //Distance vector declaration and initialization
    GrB_Vector distance;
    GrB_Vector_new(&distance, GrB_UINT64, nodes);
    GrB_Vector_setElement_UINT64(distance, 0, source); //set the start node to input source

    //Descriptor declaration and initialization
    GrB_Descriptor desc;
    GrB_Descriptor_new(&desc);
    GrB_Descriptor_set(desc, GrB_OUTP, GrB_REPLACE);
    GrB_Descriptor_set(desc, GrB_INP0, GrB_TRAN);
    GrB_Descriptor_set(desc, GrB_MASK, GrB_SCMP);

    //Constant setup
    int64_t dist = 0;
    GrB_Index nvals = 1000000;

    //Traverse graph
    while (nvals > 0) {
        ++dist;
        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL, GrB_LOR, visited, frontier, GrB_NULL); //keep track of visited nodes
        GrB_mxv(frontier, visited, GrB_NULL, GxB_LOR_LAND_BOOL, graph, frontier, desc);
        GrB_Vector_assign_UINT64(distance, frontier, GrB_NULL, dist, GrB_ALL, nodes, GrB_NULL);
        GrB_Vector_nvals(&nvals, frontier);
    }

    GrB_Vector_dup(result, distance);  //copy final distance vector to result

    //Free
    GrB_Vector_free(&frontier);
    GrB_Vector_free(&visited);
    GrB_Descriptor_free(&desc);
    GrB_Vector_free(&distance);

    return GrB_SUCCESS; //GraphBLAS return
}
