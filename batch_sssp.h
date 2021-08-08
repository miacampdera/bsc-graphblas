//
// Created by Mia Pulido Campdera on 18/7/21.
//

#ifndef BATCH_SSSP_H
#define BATCH_SSSP_H
#include <GraphBLAS.h>

GrB_Info batch_sssp(
        GrB_Matrix distances,
        GrB_Matrix graph);

#endif //BATCH_SSSP_H
