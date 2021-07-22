//
// Created by Mia Pulido Campdera on 18/7/21.
//
#include "w_sssp.h"

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


    //Constant setup for SSSP
    GrB_Index nvals = 0;

    //Traverse graph
    while (nvals < nodes) {
        GrB_Vector_eWiseAdd_BinaryOp(visited, GrB_NULL, GrB_NULL, GrB_LOR, visited, frontier, GrB_NULL);
        GrB_mxv(frontier, visited, GrB_NULL, GrB_MIN_PLUS_SEMIRING_UINT64, graph, frontier, desc);
        GrB_Vector_eWiseAdd_BinaryOp(distance, GrB_NULL, GrB_NULL, GrB_PLUS_UINT64, distance, frontier, GrB_NULL);
        GrB_Vector_nvals(&nvals, visited);
    }

    GrB_Vector_dup(result, distance); //copy final distance vector (levels) to result

    GrB_Vector_free(&distance);
    GrB_Vector_free(&visited);
    GrB_Vector_free(&frontier);
    GrB_Descriptor_free(&desc);

    return GrB_SUCCESS;
}

