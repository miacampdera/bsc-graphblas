//
// Created by Mia Pulido Campdera on 16/7/21.
//

#include "batch_sssp.h"
#include "uw_sssp.h"

GrB_Info map_add(GrB_Index node, GrB_Vector vector) {
    //TODO: add new node to map
}

GrB_Info map_contains(GrB_Index node) {
    //TODO: check if node is in map
}

GrB_Info map_get(GrB_Index node) {
    //TODO: return vector corresponding to node
}


GrB_Info batch_sssp(GrB_Vector result, GrB_Matrix graph) {

    GrB_Index nodes;
    GrB_Matrix_nrows(&nodes, graph); //extract number of nodes from input matrix

    for (int node = 0; node < nodes; ++node) {
        GrB_Vector distance;
        GrB_Vector_new(&distance, GrB_UINT64, nodes);
        if (map_contains(node)) {
            GrB_Vector known;
            GrB_Vector_new(&known, GrB_UINT64, nodes);
            GrB_Vector_dup(known, map_get(node));
            //TODO: add dist from node to curr to vector
            GrB_Vector_eWiseAdd_BinaryOp(distance, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, distance, known, GrB_NULL);

        } else {
            map_add(node, uw_sssp(result, node, graph));
        }
    }
}