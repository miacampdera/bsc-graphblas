//
// Created by Mia Pulido Campdera on 18/7/21.
//

#include "batch_sssp.h"
#include "uw_sssp.h"
#include "map.h"

GrB_Info batch_sssp(GrB_Vector distances[], GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph); //extract number of nodes from input matrix

    for (int node = 0; node < nodes; ++node) {
        GrB_Vector distance;
        GrB_Vector_new(&distance, GrB_UINT64, nodes);
        if (map_contains(&distances, node)) {
            GrB_Vector_dup(distance, map_get(&distances, node));
            //TODO: add dist from node to curr to vector
            GrB_Vector_eWiseAdd_BinaryOp(distance, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, distance, distance, GrB_NULL);

        } else {
            uw_sssp(distance, node, graph);
        }
        map_add(&distances, node, distance);
    }
}
