//
// Created by Mia Pulido Campdera on 18/7/21.
//

#include "batch_sssp.h"
#include "uw_sssp.h"
#include "w_sssp.h"


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
