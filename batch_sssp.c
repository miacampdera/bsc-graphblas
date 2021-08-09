//
// Created by Mia Pulido Campdera on 18/7/21.
//

#include "batch_sssp.h"
#include "uw_sssp.h"
#include "w_sssp.h"

/* \brief Batch single source shortest path algorithm. Calculates the shortest distance from every node
 * to every other node in the graph.
 *
 * @param[in]   distances  empty matrix to fill in with calculated distances (graphblas matrix)
 *              graph      unweighted adjacency matrix (graphblas matrix)
 *
 * @param[out]  distances  matrix of distances, row index corresponds to node index, rows correspond to
 * the calculated distance vector for that node.
 */
GrB_Info batch_sssp(GrB_Matrix distances, GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph); //extract number of nodes from input matrix

    GrB_Matrix result;

    GrB_Matrix_new(&result, GrB_UINT64, nodes, nodes);

    int64_t row = 0;

    for (int node = 0; node < nodes; ++node) {
        GrB_Index source = node;
        GrB_Vector distance;
        GrB_Vector_new(&distance, GrB_UINT64, nodes);
        uw_sssp(&distance, source, graph);
        for (int i = 0; i < nodes; ++i) {
            int64_t value;
            GrB_Vector_extractElement_UINT64(&value, distance, i);
            GrB_Matrix_setElement_UINT64(result, value, row, i);
        }
        ++row;
    }

    GrB_Matrix_dup(distances, result);

    //Free
    GrB_Matrix_free(&result);

    return GrB_SUCCESS; //GraphBLAS return
}
