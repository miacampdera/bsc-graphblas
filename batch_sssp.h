//
// Created by Mia Pulido Campdera on 16/7/21.
//

#ifndef BATCH_SSSP_H
#define BATCH_SSSP_H
#include <GraphBLAS.h>

    GrB_Info batch_sssp(
            GrB_Vector result,
            GrB_Matrix graph);

#endif //BATCH_SSSP_H
